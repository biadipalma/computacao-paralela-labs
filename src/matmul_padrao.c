/* Item 3 - Multiplicacao de Matrizes - Versao Canonica (sem blocagem) */
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
    double *C = calloc((size_t)N * N, sizeof(double));
    if (!A || !B || !C) {
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

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double soma = 0.0;
            for (int k = 0; k < N; k++) {
                soma += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = soma;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &ts_fim);
    double tempo = (ts_fim.tv_sec - ts_ini.tv_sec) +
                   (ts_fim.tv_nsec - ts_ini.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (tempo * 1e9);

    printf("[Matmul Padrao] N: %d | Tempo: %.4f s | GFLOPS: %.2f\n", N, tempo, gflops);

    /* Evita que o compilador elimine o calculo, e serve de checksum */
    double checksum = 0.0;
    for (long idx = 0; idx < (long)N * N; idx++) checksum += C[idx];
    printf("Checksum: %.6f\n", checksum);

    free(A);
    free(B);
    free(C);
    return 0;
}
