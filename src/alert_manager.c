#include "../include/common.h"
#include <signal.h>
#include <time.h>

mqd_t mq;

void handle_sigusr1(int sig) {
    printf("\nC1 received SIGUSR1 - Emergency Stop!\n");
    mq_close(mq);
    mq_unlink(QUEUE_NAME);
    exit(0);
}

void* sensor_simulator(void* arg) {

    char* disaster_types[] = {
        "Fire", "Flood", "Earthquake", "Building Collapse", "Gas Leak"
    };

    while(1) {

        int type = rand() % 5;
        int level = rand() % 5;
        int zone = rand() % 4 + 1;

        char* severity;

        if(level <= 1)
            severity = "LOW";
        else if(level <= 3)
            severity = "MEDIUM";
        else
            severity = "HIGH";

        char msg[MAX_MSG_SIZE];

        sprintf(msg,
            "Disaster: %s | Severity: %s | Zone: %d",
            disaster_types[type], severity, zone
        );

        printf("C1 Sent: %s\n", msg);

        mq_send(mq, msg, strlen(msg)+1, 0);

        sleep(2);
    }
}

int main() {

    signal(SIGUSR1, handle_sigusr1);
    srand(time(NULL));

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;

    mq = mq_open(QUEUE_NAME, O_CREAT | O_WRONLY, 0644, &attr);

    pthread_t t1;
    pthread_create(&t1, NULL, sensor_simulator, NULL);

    pthread_join(t1, NULL);

    mq_close(mq);
    mq_unlink(QUEUE_NAME);

    return 0;
}
