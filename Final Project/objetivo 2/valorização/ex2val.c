#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>


void merge2(int fi1, int fi2, int fo) {
    char buffer1[17]; // Tamanho máximo dos registos
    char buffer2[17];

    ssize_t bytesRead1, bytesRead2;

    bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
    bytesRead2 = read(fi2, buffer2, sizeof(buffer2));

    while (bytesRead1 > 0 && bytesRead2 > 0) {
        int cmp = strcmp(buffer1, buffer2);

        if (cmp < 0) {
            write(fo, buffer1, bytesRead1);
            bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
        } else {
            write(fo, buffer2, bytesRead2);
            bytesRead2 = read(fi2, buffer2, sizeof(buffer2));
        }
    }

    while (bytesRead1 > 0) {
        write(fo, buffer1, bytesRead1);
        bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
    }

    while (bytesRead2 > 0) {
        write(fo, buffer2, bytesRead2);
        bytesRead2 = read(fi2, buffer2, sizeof(buffer2));
    }
}


int mergeN(int fi[], int n, int fo) {
    if (n == 1) {
        // Nada para fundir
        return 0;
    } else if (n == 2) {
        // Fundir diretamente
        merge2(fi[0], fi[1], fo);
        return 0;
    } else {
        // Dividir a tarefa
        int mid = n / 2;
        int pipe1[2], pipe2[2];

        if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
            perror("Erro ao criar pipes");
            return 1;
        }

        pid_t pid1 = fork();
        if (pid1 == -1) {
            perror("Erro ao criar o primeiro processo filho");
            return 1;
        } else if (pid1 == 0) {
            close(pipe1[0]);
            close(pipe2[0]);
            mergeN(fi, mid, pipe1[1]);
            close(pipe1[1]);
            _exit(0);
        }

        pid_t pid2 = fork();
        if (pid2 == -1) {
            perror("Erro ao criar o segundo processo filho");
            return 1;
        } else if (pid2 == 0) {
            close(pipe1[0]);
            close(pipe2[0]);
            mergeN(fi + mid, n - mid, pipe2[1]);
            close(pipe2[1]);
            _exit(0);
        }

        close(pipe1[1]);
        close(pipe2[1]);

        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);

        merge2(pipe1[0], pipe2[0], fo);

        close(pipe1[0]);
        close(pipe2[0]);

        return 0;
    }
}

int main() {
    int files[8];
    files[0] = open("f1.txt", O_RDONLY);
    files[1] = open("f2.txt", O_RDONLY);
    files[2] = open("f3.txt", O_RDONLY);
    files[3] = open("f4.txt", O_RDONLY);
    files[4] = open("f5.txt", O_RDONLY);
    files[5] = open("f6.txt", O_RDONLY);
    files[6] = open("f7.txt", O_RDONLY);
    files[7] = open("f8.txt", O_RDONLY);

    int numFiles = sizeof(files) / sizeof(files[0]);
    int output = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

    mergeN(files, numFiles, output);
    
     printf("Fusão Concluida!\n");

    // Fechar descritores de arquivo
    for (int i = 0; i < numFiles; ++i) {
        close(files[i]);
    }
    close(output);

    return 0;
}
