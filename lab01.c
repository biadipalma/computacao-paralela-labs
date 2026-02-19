#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int N = 4;  // Num de filhos
    int i;

    for (i = 0; i < N; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            // erro fork
            perror("Erro no fork");
            exit(1);
        } 
        else if (pid == 0) {
            // Código do processo filho
            printf("Filho %d: PID = %d, PID do pai = %d\n",
                   i + 1, getpid(), getppid());
            exit(0);  // Filho termina aqui
        }
    }

    // pai espera todos os filhos terminarem
    for (i = 0; i < N; i++) {
        wait(NULL);
    }

    printf("Pai: Todos os filhos terminaram. PID = %d\n", getpid());

    return 0;
}
