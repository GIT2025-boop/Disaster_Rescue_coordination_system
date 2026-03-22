#include "../include/common.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

#define PRINT_INTERVAL 5

typedef struct {
    int total_disasters;
    int high_priority;
    int medium_priority;
    int low_priority;

    int team_count[NUM_TEAMS];
    int zone_count[NUM_ZONES];
} disaster_stats_t;

void handle_sigusr1(int sig) {
    printf("\nC3 received SIGUSR1 - Emergency Stop!\n");
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_NAME);
    exit(0);
}

void* monitor(void* arg) {

    int fd = open("/tmp/rescue_pipe", O_RDONLY);
    if (fd < 0) {
        perror("FIFO open failed");
        exit(1);
    }

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(disaster_stats_t));

    disaster_stats_t* stats = mmap(0, sizeof(disaster_stats_t),
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED, shm_fd, 0);

    sem_t* sem = sem_open(SEM_NAME, O_CREAT, 0666, 1);

    memset(stats, 0, sizeof(disaster_stats_t));

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
            else
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

            FILE *log = fopen("../logs/disaster_log.txt", "a");
            if (log) {
                fprintf(log, "%s\n", buffer);
                fclose(log);
            }

            counter++;

            if (counter % PRINT_INTERVAL == 0) {

                sem_wait(sem);

                printf("===== Disaster Stats =====\n");
                printf("Total: %d | HIGH: %d | MEDIUM: %d | LOW: %d\n",
                       stats->total_disasters,
                       stats->high_priority,
                       stats->medium_priority,
                       stats->low_priority);

                printf("Team Stats: T1:%d T2:%d T3:%d\n",
                       stats->team_count[0],
                       stats->team_count[1],
                       stats->team_count[2]);

                printf("Zone Stats: Z1:%d Z2:%d Z3:%d Z4:%d\n",
                       stats->zone_count[0],
                       stats->zone_count[1],
                       stats->zone_count[2],
                       stats->zone_count[3]);

                sem_post(sem);
            }
        }

        sleep(1);
    }
}

int main() {

    signal(SIGUSR1, handle_sigusr1);

    pthread_t t1;
    pthread_create(&t1, NULL, monitor, NULL);
    pthread_join(t1, NULL);

    return 0;
}
