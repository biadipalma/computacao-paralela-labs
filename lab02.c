#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sys/wait.h>
#include <unistd.h>

#define VECTOR_SIZE 200000000
#define NUM_PROCESSES 4

void heavy_work(double *vector, int start, int end) {
    for (int i = start; i < end; ++i) {
        vector[i] = sin(vector[i]) * cos(vector[i]) + sqrt(vector[i]);
    }
}

int main() {

    double *vector = (double *) malloc(VECTOR_SIZE * sizeof(double));
    if (vector == NULL) {
        printf("Erro ao alocar memória\n");
        return 1;
    }

    for (int i = 0; i < VECTOR_SIZE; i++)
        vector[i] = (double) i;

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);

    int chunk = VECTOR_SIZE / NUM_PROCESSES;

    for (int i = 0; i < NUM_PROCESSES; i++) {

        pid_t pid = fork();

        if (pid == 0) {  // Filho

            int begin = i * chunk;
            int finish = (i == NUM_PROCESSES - 1) ? VECTOR_SIZE : begin + chunk;

            heavy_work(vector, begin, finish);

            exit(0);
        }
    }

    for (int i = 0; i < NUM_PROCESSES; i++) {
        wait(NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    double time_parallel =
        (end.tv_sec - start.tv_sec) +
        (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Versao paralela executou em %f segundos\n", time_parallel);

    free(vector);
    return 0;
}