#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

void merge2(int fi1, int fi2, int fo) {
    char buffer1[17]; // Tamanho máximo do registo
    char buffer2[17];
    
    ssize_t bytesRead1, bytesRead2;

    bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
    bytesRead2 = read(fi2, buffer2, sizeof(buffer2));

    while (bytesRead1 > 0 && bytesRead2 > 0) {
        // Comparação dos registos como strings
        int cmp = strcmp(buffer1, buffer2);

        if (cmp < 0) {
            write(fo, buffer1, bytesRead1);
            bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
        } else {
            write(fo, buffer2, bytesRead2);
            bytesRead2 = read(fi2, buffer2, sizeof(buffer2));
        }
    }

    // Escrever o restante dos registos
    while (bytesRead1 > 0) {
        write(fo, buffer1, bytesRead1);
        bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
    }

    while (bytesRead2 > 0) {
        write(fo, buffer2, bytesRead2);
        bytesRead2 = read(fi2, buffer2, sizeof(buffer2));
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

int main() {
    int files[4];
    files[0] = open("f1.txt", O_RDONLY);
    files[1] = open("f2.txt", O_RDONLY);
    files[2] = open("f3.txt", O_RDONLY);
    files[3] = open("f4.txt", O_RDONLY);

    int outputFile = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (files[0] == -1 || files[1] == -1 || files[2] == -1 || files[3] == -1 || outputFile == -1) {
        perror("Error opening files");
        return 1;
    }

    merge4(files, outputFile);

    printf("Merge successful!\n");

    close(outputFile);
    for (int i = 0; i < 4; ++i) {
        close(files[i]);
    }

    return 0;
}
