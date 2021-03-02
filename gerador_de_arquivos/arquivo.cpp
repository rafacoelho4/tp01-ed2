#include "arquivo.hpp"
#include "../tipos.hpp"

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

void criarArquivoCrescente (int tamanho ) {
    FILE *arq;
    Item item, leitura;
    int i;

    arq = fopen("crescente.bin", "wb");

    if(arq == NULL) {
        printf("Erro ao criar arquivo");
    }

    for(i = 1; i <= tamanho; i++) {
        item.chave = i;
        item.dado1 = i * 20;
        
        char str[501];
        str[500] = '\1' ; // make the last character non-zero so we can test based on it later
        rand_str(str, sizeof str - 1);

        char cod[4];
        //itoa(i, cod, 10);
        snprintf(cod,10,"%d", i);
        strcat(str, cod);
        strcpy(item.dado2, str);
        
        fwrite(&item, sizeof(Item), 1, arq);
    }

    fclose(arq);

    FILE *newf;

    newf = fopen("crescente.bin", "rb");
    if(newf == NULL) {
        printf("Erro ao ler arquivo");
    }

   /*  // LEITURA FUNCIONANDO
    i = 0;
    while (fread(&leitura, sizeof(Item), 1, newf) == 1) {
        // printf("\n\nIndex: %d\n", i + 1);
        printf("\n\nChave: %d\n", leitura.chave);
        printf("Dado 1: %ld\n", leitura.dado1);
        printf("Dado 2: %s", leitura.dado2);
        if(i > 10) break;
        i++;
    } */

    fclose(newf);
}

void criarArquivoDecrescente (int tamanho) {
    FILE *arq;
    Item item, leitura;

    arq = fopen("decrescente.bin", "wb");

    if(arq == NULL) {
        printf("Erro ao criar arquivo");
    }

    int j = 0;
    for(int i = tamanho; i > 0; i--) {
        item.chave = i;
        item.dado1 = i * 20;
        
        char str[501];
        str[500] = '\1'; // make the last character non-zero so we can test based on it later
        rand_str(str, sizeof str - 1);

        char cod[8];
        //itoa(i, cod, 10);
        snprintf(cod,10,"%d", i);
        strcat(str, cod);
        strcpy(item.dado2, str);

        // if (i < TAMANHO;&& i > 999990) printf("\n\n%d : %s\n\t\n\n%s\n\n %s", i, cod, str, item.dado2);
        // if (i < TAMANHO && i > 80) printf("\n\n%d : %s\n\t\n%s\n\n %s", i, cod, str, item.dado2);
        
        
        fwrite(&item, sizeof(Item), 1, arq);
        j++;
    }

    fclose(arq);

    FILE *newff;

    newff = fopen("decrescente.bin", "rb");
    if(newff == NULL) {
        printf("Erro ao ler arquivo");
    }

    // LEITURA FUNCIONANDO
    int i = 0;
  /*   while (fread(&leitura, sizeof(Item), 1, newff) == 1) {
        // printf("\n\nIndex: %d\n", i + 1);
        printf("\n\nChave: %d\n", leitura.chave);
        printf("Dado 1: %ld\n", leitura.dado1);
        printf("Dado 2: %s", leitura.dado2);
        if(i > 10) break;
        i++;
    } */

    fclose(newff);
}

void criarArquivoAleatorio(int tamanho) {
    FILE *arq;
    Item item, leitura;
    int i;

    arq = fopen("aleatorio.bin", "wb");

    if(arq == NULL) {
        printf("Erro ao criar arquivo");
    }

    srand(time(NULL)); // Initialization, should only be called once.

    for(i = 1; i <= tamanho; i++) {
        item.dado1 = i * 20;

        item.chave= rand()%tamanho; // Returns a pseudo-random integer between 0 and RAND_MAX.
        //printf("%i\n",item.chave);
        char str[501];
        str[500] = '\1'; // Make the last character non-zero so we can test based on it later
        rand_str(str, sizeof str - 1);

        char cod[4];
        //itoa(i, cod, 10);
        snprintf(cod,10,"%d", i);
        strcat(str, cod);
        strcpy(item.dado2, str);
        
        fwrite(&item, sizeof(Item), 1, arq);
    }

    fclose(arq);
}

int* criaChaves(int qtd){
    srand(time(NULL));
    int* chaves = (int*) malloc (qtd * sizeof(int));

    for (int i = 0; i < qtd; i++){
        chaves[i] = i;  
    }
     
    // "Shuffle" dos nºs no vetor, impedindo repetições
    for (int i = 0; i < qtd; i++){
        int r = i + (rand() % (qtd - i)); //int r = i + (rand() % (qtd - i)); 
        int temp = chaves[i]; 
        chaves[i] = chaves[r]; 
        chaves[r] = temp;
    }

    // Impressão para teste
    printf("CHAVES:\n");
    for(int i = 0; i < 10; i++){
        printf("%d ", chaves[i]);
    }
    printf("\n");

    return chaves;
}
