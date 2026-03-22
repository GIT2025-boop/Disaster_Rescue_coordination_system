#include "../include/common.h"

mqd_t mq;

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

        printf("\n[C2 RESCUE COORDINATOR]\nReceived -> %s\n", buffer);

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

        printf("Team Assigned -> %d\n", team);

        write(fd, new_msg, strlen(new_msg)+1);

        sleep(2);
    }
}

int main() {

    mq = mq_open(QUEUE_NAME, O_RDONLY);

    if (mq < 0) {
        perror("mq_open failed");
        exit(1);
    }

    pthread_t t1;
    pthread_create(&t1, NULL, receiver, NULL);
    pthread_join(t1, NULL);

    mq_close(mq);

    return 0;
}
