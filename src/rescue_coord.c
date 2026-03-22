#include "../include/common.h"
#include <signal.h>
#include <fcntl.h>

mqd_t mq;

void handle_sigusr1(int sig) {
    printf("\nC2 received SIGUSR1 - Emergency Stop!\n");
    exit(0);
}

void* receiver(void* arg) {

    int fd = open("/tmp/rescue_pipe", O_WRONLY);

    if (fd < 0) {
        perror("FIFO open failed");
        exit(1);
    }

    char buffer[MAX_MSG_SIZE];

    while(1) {

        int bytes = mq_receive(mq, buffer, MAX_MSG_SIZE, NULL);

        if (bytes < 0) {
            perror("mq_receive failed");
            continue;
        }

        buffer[bytes] = '\0';

        printf("C2 Received: %s\n", buffer);

        int team;

        if (strstr(buffer, "HIGH"))
            team = 1;
        else if (strstr(buffer, "MEDIUM"))
            team = 2;
        else
            team = 3;

        char new_msg[MAX_MSG_SIZE];

        snprintf(new_msg, MAX_MSG_SIZE,
                 "%.200s | Team Assigned: %d", buffer, team);

        printf("Allocating Rescue Team %d...\n", team);

        write(fd, new_msg, strlen(new_msg)+1);

        sleep(2);
    }
}

int main() {

    signal(SIGUSR1, handle_sigusr1);

    mq = mq_open(QUEUE_NAME, O_RDONLY);

    pthread_t t1;
    pthread_create(&t1, NULL, receiver, NULL);

    pthread_join(t1, NULL);

    mq_close(mq);

    return 0;
}
