#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arquivo.h"

void rand_str(char *dest, size_t length) {
    char charset[] = "0123456789"
                     "abcdefghijklmnopqrstuvwxyz"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    while (length-- > 0) {
        size_t index = (double) rand() / RAND_MAX * (sizeof charset - 1);
        *dest++ = charset[index];
    }
    *dest = '\0';
}

void criarArquivoCrescente () {
    FILE *arq;
    Item item, leitura;
    int i;

    arq = fopen("crescente.bin", "wb");

    if(arq == NULL) {
        printf("Erro ao criar arquivo");
    }

    for(i = 1; i <= 1000000; i++) {
        item.chave = i;
        item.dado1 = i * 20;
        
        char str[501] = { [500] = '\1' }; // make the last character non-zero so we can test based on it later
        rand_str(str, sizeof str - 1);

        char cod[4];
        itoa(i, cod, 10);
        strcat(str, cod);
        strcpy(item.dado2, str);
        
        fwrite(&item, sizeof(Item), 1, arq);
    }

    fclose(arq);

    FILE *new;

    new = fopen("crescente.bin", "rb");
    if(new == NULL) {
        printf("Erro ao ler arquivo");
    }

    // LEITURA FUNCIONANDO
    i = 0;
    while (fread(&leitura, sizeof(Item), 1, new) == 1) {
        // printf("\n\nIndex: %d\n", i + 1);
        printf("\n\nChave: %d\n", leitura.chave);
        printf("Dado 1: %ld\n", leitura.dado1);
        printf("Dado 2: %s", leitura.dado2);
        if(i > 10) break;
        i++;
    }

    fclose(new);
}

void criarArquivoDecrescente () {
    FILE *arq;
    Item item, leitura;

    arq = fopen("decrescente.bin", "wb");

    if(arq == NULL) {
        printf("Erro ao criar arquivo");
    }

    int j = 0;
    for(int i = 1000000; i > 0; i--) {
        item.chave = i;
        item.dado1 = i * 20;
        
        char str[501] = { [500] = '\1' }; // make the last character non-zero so we can test based on it later
        rand_str(str, sizeof str - 1);

        char cod[8];
        itoa(i, cod, 10);
        strcat(str, cod);
        strcpy(item.dado2, str);

        // if (i < 1000000 && i > 999990) printf("\n\n%d : %s\n\t\n\n%s\n\n %s", i, cod, str, item.dado2);
        // if (i < 100 && i > 80) printf("\n\n%d : %s\n\t\n%s\n\n %s", i, cod, str, item.dado2);
        
        
        fwrite(&item, sizeof(Item), 1, arq);
        j++;
    }

    fclose(arq);

    FILE *new;

    new = fopen("decrescente.bin", "rb");
    if(new == NULL) {
        printf("Erro ao ler arquivo");
    }

    // LEITURA FUNCIONANDO
    int i = 0;
    while (fread(&leitura, sizeof(Item), 1, new) == 1) {
        // printf("\n\nIndex: %d\n", i + 1);
        printf("\n\nChave: %d\n", leitura.chave);
        printf("Dado 1: %ld\n", leitura.dado1);
        printf("Dado 2: %s", leitura.dado2);
        if(i > 10) break;
        i++;
    }

    fclose(new);
}

void criarArquivoAleatorio () {
    FILE *arq;
    Item item, leitura;
    int i;

    arq = fopen("decrescente.bin", "wb");

    if(arq == NULL) {
        printf("Erro ao criar arquivo");
    }

    srand(time(NULL)); // Initialization, should only be called once.

    for(i = 1; i <= 1000000; i++) {
        item.chave = i;
        item.dado1 = i * 20;

        item.chave = rand(); // Returns a pseudo-random integer between 0 and RAND_MAX.
        
        char str[501] = { [500] = '\1' }; // make the last character non-zero so we can test based on it later
        rand_str(str, sizeof str - 1);

        char cod[4];
        itoa(i, cod, 10);
        strcat(str, cod);
        strcpy(item.dado2, str);
        
        fwrite(&item, sizeof(Item), 1, arq);
    }

    fclose(arq);
}

int main() {
    // criarArquivoCrescente();
    // criarArquivoDecrescente();
    return 0;
}