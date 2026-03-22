#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>

int main() {

    printf("===== DISASTER RESCUE CONTROL CENTER =====\n");
    printf("Starting all systems...\n");

    // Create FIFO if not exists
    if (access("/tmp/rescue_pipe", F_OK) == -1) {
        mkfifo("/tmp/rescue_pipe", 0666);
    }

    pid_t p1 = fork();
    if (p1 == 0) {
        execl("./build/c1", "c1", NULL);
    }

    pid_t p2 = fork();
    if (p2 == 0) {
        execl("./build/c2", "c2", NULL);
    }

    pid_t p3 = fork();
    if (p3 == 0) {
        execl("./build/c3", "c3", NULL);
    }

    while(1) {
        printf("Control Center Monitoring System...\n");
        sleep(5);

        printf("\nSending SIGUSR1 to all processes...\n");

        kill(p1, SIGUSR1);
        kill(p2, SIGUSR1);
        kill(p3, SIGUSR1);

        break;
    }

    return 0;
}
