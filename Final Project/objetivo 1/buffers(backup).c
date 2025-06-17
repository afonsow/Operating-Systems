#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

void merge2(int fi1, int fi2, int fo) {
    char buffer1[17]; // Tamanho máximo do registro (ajuste conforme necessário)
    char buffer2[17];
    
    ssize_t bytesRead1, bytesRead2;

    bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
    bytesRead2 = read(fi2, buffer2, sizeof(buffer2));

    while (bytesRead1 > 0 && bytesRead2 > 0) {
        // Comparação dos registros como strings
        int cmp = strcmp(buffer1, buffer2);

        if (cmp < 0) {
            write(fo, buffer1, bytesRead1);
            bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
        } else {
            write(fo, buffer2, bytesRead2);
            bytesRead2 = read(fi2, buffer2, sizeof(buffer2));
        }
    }

    // Escrever o restante dos registros
    while (bytesRead1 > 0) {
        write(fo, buffer1, bytesRead1);
        bytesRead1 = read(fi1, buffer1, sizeof(buffer1));
    }

    while (bytesRead2 > 0) {
        write(fo, buffer2, bytesRead2);
        bytesRead2 = read(fi2, buffer2, sizeof(buffer2));
    }
}

int main() {
    int file1 = open("arquivo1.txt", O_RDONLY);
    int file2 = open("arquivo2.txt", O_RDONLY);
    int outputFile = open("saida.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (file1 == -1 || file2 == -1 || outputFile == -1) {
        perror("Erro ao abrir os arquivos");
        return 1;
    }

    merge2(file1, file2, outputFile);

    printf("Fusão bem-sucedida!\n");

    close(file1);
    close(file2);
    close(outputFile);

    return 0;
}
