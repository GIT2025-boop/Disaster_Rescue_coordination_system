#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <signal.h>

#define QUEUE_NAME "/disaster_queue"
#define MAX_MSG_SIZE 256

#define SHM_NAME "/disaster_shm"
#define SEM_NAME "/disaster_sem"

#define NUM_TEAMS 3
#define NUM_ZONES 4

#endif
