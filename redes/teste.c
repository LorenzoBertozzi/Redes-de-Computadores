#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#define THREAD_COUNT 5
int p[THREAD_COUNT] = {0, 1, 2, 3, 4};

void *process(void *arg);

int main(int argc, char **argv) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <hostname> <filename>\n", argv[0]);
        return 1;
    }

    void *status;
    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_create(&threads[i], NULL, process, argv);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], &status);
    }

    return 0;
}

void *process(void *arg) {
    char **argv = (char **)arg;
    char command[256];

    snprintf(command, sizeof(command), "./client %s %s", argv[1], argv[2]);
    system(command);

    return NULL;
}
