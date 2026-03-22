#include "../include/common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <pthread.h>

#define PRINT_INTERVAL 5  // print stats every 5 messages
#define NUM_TEAMS 3
#define NUM_ZONES 4

typedef struct {
    int total_disasters;
    int high_priority;
    int medium_priority;
    int low_priority;

    int team_count[NUM_TEAMS];   // Team 1-3 stats
    int zone_count[NUM_ZONES];   // Zone 1-4 stats
} disaster_stats_t;

void* monitor(void* arg) {
    int fd = open("/tmp/rescue_pipe", O_RDONLY);
    if (fd < 0) {
        perror("FIFO open failed");
        exit(1);
    }

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(disaster_stats_t));
    disaster_stats_t* stats = mmap(0, sizeof(disaster_stats_t),
                                   PROT_READ | PROT_WRITE, MAP_SHARED,
                                   shm_fd, 0);

    sem_t* sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    stats->total_disasters = 0;
    stats->high_priority = 0;
    stats->medium_priority = 0;
    stats->low_priority = 0;

    for(int i = 0; i < NUM_TEAMS; i++)
        stats->team_count[i] = 0;

    for(int i = 0; i < NUM_ZONES; i++)
        stats->zone_count[i] = 0;

    char buffer[MAX_MSG_SIZE];
    int counter = 0;

    while (1) {
        int bytes = read(fd, buffer, MAX_MSG_SIZE - 1);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            printf("C3 Monitoring: %s\n", buffer);
            printf("Updating Stats...\n");

            sem_wait(sem);

            stats->total_disasters++;
            if (strstr(buffer, "HIGH"))
                stats->high_priority++;
            else if (strstr(buffer, "MEDIUM"))
                stats->medium_priority++;
            else
                stats->low_priority++;

            if (strstr(buffer, "Team Assigned: 1"))
                stats->team_count[0]++;
            else if (strstr(buffer, "Team Assigned: 2"))
                stats->team_count[1]++;
            else if (strstr(buffer, "Team Assigned: 3"))
                stats->team_count[2]++;

            if (strstr(buffer, "Zone: 1"))
                stats->zone_count[0]++;
            else if (strstr(buffer, "Zone: 2"))
                stats->zone_count[1]++;
            else if (strstr(buffer, "Zone: 3"))
                stats->zone_count[2]++;
            else if (strstr(buffer, "Zone: 4"))
                stats->zone_count[3]++;

            sem_post(sem);

            FILE *log_file = fopen("../logs/disaster_log.txt", "a");
            if (log_file != NULL) {
                fprintf(log_file, "%s\n", buffer);
                fclose(log_file);
            }

            counter++;

            if (counter % PRINT_INTERVAL == 0) {
                sem_wait(sem);
                printf("===== Disaster Stats =====\n");
                printf("Total: %d | HIGH: %d | MEDIUM: %d | LOW: %d\n",
                       stats->total_disasters, stats->high_priority,
                       stats->medium_priority, stats->low_priority);

                printf("Team Stats: T1:%d T2:%d T3:%d\n",
                       stats->team_count[0], stats->team_count[1], stats->team_count[2]);

                printf("Zone Stats: Z1:%d Z2:%d Z3:%d Z4:%d\n",
                       stats->zone_count[0], stats->zone_count[1],
                       stats->zone_count[2], stats->zone_count[3]);

                FILE *summary_file = fopen("../data/stats_snapshot.txt", "w");
                if (summary_file != NULL) {
                    fprintf(summary_file, "===== Disaster Stats =====\n");
                    fprintf(summary_file, "Total: %d | HIGH: %d | MEDIUM: %d | LOW: %d\n",
                           stats->total_disasters, stats->high_priority,
                           stats->medium_priority, stats->low_priority);

                    fprintf(summary_file, "Team Stats: T1:%d T2:%d T3:%d\n",
                           stats->team_count[0], stats->team_count[1], stats->team_count[2]);

                    fprintf(summary_file, "Zone Stats: Z1:%d Z2:%d Z3:%d Z4:%d\n",
                           stats->zone_count[0], stats->zone_count[1],
                           stats->zone_count[2], stats->zone_count[3]);

                    fclose(summary_file);
                }

                sem_post(sem);
            }
        }
        sleep(1);
    }

    munmap(stats, sizeof(disaster_stats_t));
    close(shm_fd);
    shm_unlink(SHM_NAME);

    sem_close(sem);
    sem_unlink(SEM_NAME);

    return NULL;
}

int main() {
    pthread_t t1;
    pthread_create(&t1, NULL, monitor, NULL);
    pthread_join(t1, NULL);
    return 0;
}
