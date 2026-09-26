/* Item 2 - Varredura por Coluna (Column-Major) - Pobre Localidade Espacial */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);

    double *A = malloc((size_t)N * N * sizeof(double));
    double *B = malloc((size_t)N * N * sizeof(double));
    if (!A || !B) {
        fprintf(stderr, "Falha ao alocar memoria\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            B[i * N + j] = (double)(i * j);
        }
    }

    struct timespec ts_ini, ts_fim;
    clock_gettime(CLOCK_MONOTONIC, &ts_ini);

    long pares = 0;
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            if ((int)A[i * N + j] % 2 == 0) {
                pares++;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &ts_fim);
    double tempo = (ts_fim.tv_sec - ts_ini.tv_sec) +
                   (ts_fim.tv_nsec - ts_ini.tv_nsec) / 1e9;

    printf("[Varredura Coluna] N: %d | Pares: %ld | Tempo: %.6f s\n", N, pares, tempo);

    free(A);
    free(B);
    return 0;
}
