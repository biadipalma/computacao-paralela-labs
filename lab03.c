#include <stdio.h>

#define TAM_VETOR 1000

int main() {
    int vector[TAM_VETOR];
    double total_sum = 0;
    int fd[2];
    pid_t pid;

    for (int i = 0; i < TAM_VETOR; i++) {
        vector[i] = i + 1;
    }
    if (pipe(fd) == -1) {
        perror("Erro ao criar pipe");
        return 1;
    }
        pid = fork();
        
    if (pid < 0) {
        perror("Erro no fork");
        return 1;
    }
    

    for (int i = 0; i < TAM_VETOR; i++) {
        total_sum += vector[i];
    }

    printf("Soma Total: %.2f\n", total_sum);
    
    double expected = (TAM_VETOR * (TAM_VETOR + 1)) / 2.0;
    printf("Soma Esperada: %.2f\n", expected);

    return 0;
}