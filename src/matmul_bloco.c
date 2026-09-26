/* Item 3 - Multiplicacao de Matrizes - Versao com Blocagem (Tiling) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <BLOCK_SIZE>\n", argv[0]);
        return 1;
    }
    int N = atoi(argv[1]);
    int B = atoi(argv[2]);

    double *A = malloc((size_t)N * N * sizeof(double));
    double *Bm = malloc((size_t)N * N * sizeof(double));
    double *C = calloc((size_t)N * N, sizeof(double));
    if (!A || !Bm || !C) {
        fprintf(stderr, "Falha ao alocar memoria\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i * N + j] = (double)(i + j);
            Bm[i * N + j] = (double)(i * j);
        }
    }

    struct timespec ts_ini, ts_fim;
    clock_gettime(CLOCK_MONOTONIC, &ts_ini);

    for (int ii = 0; ii < N; ii += B) {
        for (int jj = 0; jj < N; jj += B) {
            for (int kk = 0; kk < N; kk += B) {
                int i_max = (ii + B < N) ? ii + B : N;
                int k_max = (kk + B < N) ? kk + B : N;
                int j_max = (jj + B < N) ? jj + B : N;
                for (int i = ii; i < i_max; i++) {
                    for (int k = kk; k < k_max; k++) {
                        double r = A[i * N + k];
                        for (int j = jj; j < j_max; j++) {
                            C[i * N + j] += r * Bm[k * N + j];
                        }
                    }
                }
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &ts_fim);
    double tempo = (ts_fim.tv_sec - ts_ini.tv_sec) +
                   (ts_fim.tv_nsec - ts_ini.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (tempo * 1e9);

    printf("[Matmul Bloco] N: %d | Bloco B: %d | Tempo: %.4f s | GFLOPS: %.2f\n", N, B, tempo, gflops);

    double checksum = 0.0;
    for (long idx = 0; idx < (long)N * N; idx++) checksum += C[idx];
    printf("Checksum: %.6f\n", checksum);

    free(A);
    free(Bm);
    free(C);
    return 0;
}
