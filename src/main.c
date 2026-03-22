#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {

    printf("===== DISASTER RESCUE CONTROL CENTER =====\n");

    printf("Starting all systems...\n");

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

    // Parent process
    while(1) {
        printf("Control Center Monitoring System...\n");
        sleep(5);
    }

    return 0;
}
