#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main() {
    int pipefd[2]; // pipefd[0] = read, pipefd[1] = write
    char buffer[100];

    // 1. Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // 2. Create a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process: Writes to the pipe
        close(pipefd[0]); // Child doesn't read, close read end
        const char *msg = "Ola do filho!";
        
        printf("Filho: enviando mensagem...\n");
        write(pipefd[1], msg, strlen(msg) + 1);
        
        close(pipefd[1]); // Close write end after use
        exit(EXIT_SUCCESS);
    } else { // Parent Process: Reads from the pipe
        close(pipefd[1]); // Parent doesn't write, close write end
        
        printf("Pai: esperando mensagem...\n");
        read(pipefd[0], buffer, sizeof(buffer));
        
        printf("Pai: recebeu a mensagem: '%s'\n", buffer);
        
        close(pipefd[0]); // Close read end after use
        wait(NULL);       // Wait for child to finish (prevents zombies)
        return 0;
    }
}