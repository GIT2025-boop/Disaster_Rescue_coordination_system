#include "../include/common.h"
#include <time.h>

#define PRINT_INTERVAL 5

typedef struct {
    int total_disasters;
    int high_priority;
    int medium_priority;
    int low_priority;
    int zone_count[NUM_ZONES];   
} disaster_stats_t;

disaster_stats_t *global_stats;
sem_t *global_sem;

//  SIGUSR1 HANDLER
void handle_sigusr1(int sig) {

    printf("\n🚨 EMERGENCY SNAPSHOT (SIGUSR1) 🚨\n");

    sem_wait(global_sem);

    printf("Total: %d\n", global_stats->total_disasters);
    printf("HIGH: %d | MEDIUM: %d | LOW: %d\n",
           global_stats->high_priority,
           global_stats->medium_priority,
           global_stats->low_priority);

    printf("Zone Stats:\n");
    for(int i = 0; i < NUM_ZONES; i++) {
        printf("Zone %d = %d\n", i+1, global_stats->zone_count[i]);
    }

    sem_post(global_sem);

    FILE *file = fopen("../data/stats_snapshot.txt", "w");
    if(file) {
        fprintf(file, "Emergency Snapshot\n");
        fprintf(file, "Total: %d\n", global_stats->total_disasters);
        fprintf(file, "HIGH: %d\n", global_stats->high_priority);
        fprintf(file, "MEDIUM: %d\n", global_stats->medium_priority);
        fprintf(file, "LOW: %d\n", global_stats->low_priority);

        for(int i = 0; i < NUM_ZONES; i++) {
            fprintf(file, "Zone %d: %d\n", i+1, global_stats->zone_count[i]);
        }

        fclose(file);
    }
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

    global_stats = stats;
    global_sem = sem;

    memset(stats, 0, sizeof(disaster_stats_t));

    char buffer[MAX_MSG_SIZE];
    int counter = 0;

    while (1) {

        int bytes = read(fd, buffer, MAX_MSG_SIZE - 1);

        if (bytes > 0) {

            buffer[bytes] = '\0';

            time_t now = time(NULL);

            printf("\n[C3 MONITORING] [%s] Monitoring -> %s\n",
                   ctime(&now), buffer);

            
            if (strstr(buffer, "HIGH")) {
                printf("🚨 CRITICAL ALERT DETECTED 🚨\n");
            }

            sem_wait(sem);

            stats->total_disasters++;

            if (strstr(buffer, "HIGH"))
                stats->high_priority++;
            else if (strstr(buffer, "MEDIUM"))
                stats->medium_priority++;
            else
                stats->low_priority++;

            int zone;
            sscanf(buffer, "%*[^|]| %*[^|]| Zone: %d", &zone);

            if(zone >= 1 && zone <= NUM_ZONES) {
                stats->zone_count[zone - 1]++;
            }

            sem_post(sem);

            counter++;

            if (counter % PRINT_INTERVAL == 0) {

                printf("\n[STATS] Total = %d\n", stats->total_disasters);

                printf("Zone Stats:\n");
                for(int i = 0; i < NUM_ZONES; i++) {
                    printf("Zone %d = %d\n", i+1, stats->zone_count[i]);
                }

                FILE *file = fopen("../data/stats_snapshot.txt", "w");
                if(file) {
                    fprintf(file, "Auto Snapshot\n");
                    fprintf(file, "Total: %d\n", stats->total_disasters);
                    fclose(file);
                }
            }
        }

        sleep(2);
    }
}

int main() {

    signal(SIGUSR1, handle_sigusr1);

    pthread_t t1;
    pthread_create(&t1, NULL, monitor, NULL);
    pthread_join(t1, NULL);

    return 0;
}
