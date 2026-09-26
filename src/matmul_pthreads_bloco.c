/* Item 4 - Multiplicacao de Matrizes: Pthreads + Blocagem (por faixas de linhas) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

static int N, BS;
static double *A, *Bm, *C;

typedef struct {
    int linha_ini;
    int linha_fim;
} TarefaThread;

void *multiplica_faixa_blocada(void *arg) {
    TarefaThread *t = (TarefaThread *)arg;
    for (int ii = t->linha_ini; ii < t->linha_fim; ii += BS) {
        int i_max = (ii + BS < t->linha_fim) ? ii + BS : t->linha_fim;
        for (int jj = 0; jj < N; jj += BS) {
            int j_max = (jj + BS < N) ? jj + BS : N;
            for (int kk = 0; kk < N; kk += BS) {
                int k_max = (kk + BS < N) ? kk + BS : N;
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
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Uso: %s <N> <T threads> <BLOCK_SIZE>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    int T = atoi(argv[2]);
    BS = atoi(argv[3]);

    A = malloc((size_t)N * N * sizeof(double));
    Bm = malloc((size_t)N * N * sizeof(double));
    C = calloc((size_t)N * N, sizeof(double));
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

    pthread_t *threads = malloc(T * sizeof(pthread_t));
    TarefaThread *tarefas = malloc(T * sizeof(TarefaThread));
    int linhas_por_thread = N / T;

    struct timespec ts_ini, ts_fim;
    clock_gettime(CLOCK_MONOTONIC, &ts_ini);

    for (int t = 0; t < T; t++) {
        tarefas[t].linha_ini = t * linhas_por_thread;
        tarefas[t].linha_fim = (t == T - 1) ? N : (t + 1) * linhas_por_thread;
        pthread_create(&threads[t], NULL, multiplica_faixa_blocada, &tarefas[t]);
    }
    for (int t = 0; t < T; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &ts_fim);
    double tempo = (ts_fim.tv_sec - ts_ini.tv_sec) +
                   (ts_fim.tv_nsec - ts_ini.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (tempo * 1e9);

    printf("[Matmul Pthreads+Bloco] N: %d | Threads: %d | Bloco: %d | Tempo: %.4f s | GFLOPS: %.2f\n",
           N, T, BS, tempo, gflops);

    double checksum = 0.0;
    for (long idx = 0; idx < (long)N * N; idx++) checksum += C[idx];
    printf("Checksum: %.6f\n", checksum);

    free(A); free(Bm); free(C);
    free(threads); free(tarefas);
    return 0;
}
