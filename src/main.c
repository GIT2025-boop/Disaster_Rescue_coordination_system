#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <mqueue.h>

pid_t p1, p2, p3;

void cleanup(int sig) {

    printf("\nCleaning resources...\n");

    unlink("/tmp/rescue_pipe");
    mq_unlink("/disaster_queue");

    kill(p1, SIGKILL);
    kill(p2, SIGKILL);
    kill(p3, SIGKILL);

    exit(0);
}

int main() {

    signal(SIGINT, cleanup);

    printf("===== DISASTER RESCUE CONTROL CENTER =====\n");

    unlink("/tmp/rescue_pipe");
    if (mkfifo("/tmp/rescue_pipe", 0666) < 0) {
        perror("mkfifo failed");
    }

    p1 = fork();
    if (p1 == 0)
        execl("./build/c1", "c1", NULL);

    p2 = fork();
    if (p2 == 0)
        execl("./build/c2", "c2", NULL);

    p3 = fork();
    if (p3 == 0)
        execl("./build/c3", "c3", NULL);

    while(1) {

        printf("\n[CONTROL CENTER RUNNING]\n");

        sleep(10);

        printf("\n[CONTROL] Sending SIGUSR1 to Monitoring...\n");

        kill(p3, SIGUSR1);
    }

    return 0;
}
