#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void merge2(int input1, int input2, int output);
int merge4(int fi[4], int fo);


void merge2(int input1, int input2, int output) {
    char buffer1[17];
    char buffer2[17];

    ssize_t bytesRead1, bytesRead2;

    bytesRead1 = read(input1, buffer1, sizeof(buffer1));
    bytesRead2 = read(input2, buffer2, sizeof(buffer2));

    while (bytesRead1 > 0 && bytesRead2 > 0) {
        // Comparação dos registos como strings
        int cmp = strcmp(buffer1, buffer2);

        if (cmp < 0) {
            write(output, buffer1, bytesRead1);
            bytesRead1 = read(input1, buffer1, sizeof(buffer1));
        } else {
            write(output, buffer2, bytesRead2);
            bytesRead2 = read(input2, buffer2, sizeof(buffer2));
        }
    }

    // Escrever o restante dos registos
    while (bytesRead1 > 0) {
        write(output, buffer1, bytesRead1);
        bytesRead1 = read(input1, buffer1, sizeof(buffer1));
    }

    while (bytesRead2 > 0) {
        write(output, buffer2, bytesRead2);
        bytesRead2 = read(input2, buffer2, sizeof(buffer2));
    }
}

int merge4(int fi[4], int fo) {
    int pipe1[2], pipe2[2];

    // Criar pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Erro ao criar pipes");
        return 1;
    }

    // Primeiro processo filho para fundir fi[0] e fi[1]
    pid_t pid1 = fork();
    if (pid1 == -1) {
        perror("Erro ao criar o primeiro processo filho");
        return 1;
    } else if (pid1 == 0) {
        // Processo filho
        close(pipe1[0]); 
        merge2(fi[0], fi[1], pipe1[1]);
        close(pipe1[1]); 
        _exit(0);
    }

    // Segundo processo filho para fundir fi[2] e fi[3]
    pid_t pid2 = fork();
    if (pid2 == -1) {
        perror("Erro ao criar o segundo processo filho");
        return 1;
    } else if (pid2 == 0) {
        // Processo filho
        close(pipe2[0]); 
        merge2(fi[2], fi[3], pipe2[1]);
        close(pipe2[1]); 
        _exit(0);
    }

    // Processo pai
    close(pipe1[1]); 
    close(pipe2[1]); 

    // Esperar pelo fim dos processos filhos
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);

    // Fundir os resultados ordenados dos dois pipes
    merge2(pipe1[0], pipe2[0], fo);

    close(pipe1[0]); 
    close(pipe2[0]); 

    return 0;
}

int main(int argc, char *argv[]) {

    printf("\n");  // Para os registos não ficarem colados aos comandos
    printf("\n");
    	
    if (argc != 5) {
        printf("Uso: %s file1 file2 file3 file4\n", argv[0]);
        return 1;
    }

    // Descritores dos ficheiros ordenados
    int sortedFiles[4];

    // Usar os pipes para capturar o output dos ficheiros ordenados
    for (int i = 0; i < 4; ++i) {
        int pipefd[2];
        if (pipe(pipefd) == -1) {
            perror("Erro ao criar o pipe");
            return 1;
        }

        pid_t pid = fork();
        if (pid == -1) {
            perror("Erro ao criar processo filho");
            return 1;
        } else if (pid == 0) {
            // Processo filho
            close(pipefd[0]); 

            // Redirecionar stdout para o pipe
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]); 

            // Executar comando sort no ficheiro
            execlp("sort", "sort", argv[i + 1], NULL);
            perror("Erro a executar comando sort");
            _exit(1);
        } else {
            // Processo pai
            close(pipefd[1]); 
            sortedFiles[i] = pipefd[0]; 
        }
    }

    // Esperar pelo fim dos processos filhos
    for (int i = 0; i < 4; ++i) {
        wait(NULL);
    }

    merge4(sortedFiles, STDOUT_FILENO);

    for (int i = 0; i < 4; ++i) {
        close(sortedFiles[i]);
    }


    printf("\n");  // Para os registos não ficarem colados aos comandos
    printf("\n");
    	
    return 0;
}
