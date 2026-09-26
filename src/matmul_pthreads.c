/* Item 4 - Multiplicacao de Matrizes Paralela com Pthreads (particao por linhas) */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

static int N;
static double *A, *Bm, *C;

typedef struct {
    int linha_ini;
    int linha_fim;
} TarefaThread;

void *multiplica_faixa(void *arg) {
    TarefaThread *t = (TarefaThread *)arg;
    for (int i = t->linha_ini; i < t->linha_fim; i++) {
        for (int j = 0; j < N; j++) {
            double soma = 0.0;
            for (int k = 0; k < N; k++) {
                soma += A[i * N + k] * Bm[k * N + j];
            }
            C[i * N + j] = soma;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <N> <T threads>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    int T = atoi(argv[2]);

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
        pthread_create(&threads[t], NULL, multiplica_faixa, &tarefas[t]);
    }
    for (int t = 0; t < T; t++) {
        pthread_join(threads[t], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &ts_fim);
    double tempo = (ts_fim.tv_sec - ts_ini.tv_sec) +
                   (ts_fim.tv_nsec - ts_ini.tv_nsec) / 1e9;
    double gflops = (2.0 * N * N * N) / (tempo * 1e9);

    printf("[Matmul Pthreads] N: %d | Threads: %d | Tempo: %.4f s | GFLOPS: %.2f\n", N, T, tempo, gflops);

    double checksum = 0.0;
    for (long idx = 0; idx < (long)N * N; idx++) checksum += C[idx];
    printf("Checksum: %.6f\n", checksum);

    free(A); free(Bm); free(C);
    free(threads); free(tarefas);
    return 0;
}
