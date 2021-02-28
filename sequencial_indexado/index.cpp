#include <stdio.h>
#include <time.h>
#include "index.hpp"

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void sequencial_indexado(int situacao, int chave, int qtd, bool parametro) {
    FILE *arq;
    // Nossa tabela consiste em números inteiros TAM_DO_ARQ / REG_POR_PAG
    int tabela[MAXTABELA];

    // Parâmetros de análise do algoritmo
    int comparacoes = 0, transferencias = 0; double tempo1, tempo2;

    // POS = Tamanho da tabela de índices // RESULTADO = Se o item foi encontrado ou não
    int pos; int resultado;

    // Colocamos a chave desejada em um Item e retornamos o registro completo, se encontrado.
    Item x; x.chave = chave;

    if(situacao == 1) {
        if ((arq = fopen("crescente.bin","rb")) == NULL) {
            printf("Erro ao abrir arquivo"); 
            return;
        }
        pos = preprocessamento(tabela, arq, qtd, &transferencias, &tempo1, parametro);
        resultado = pesquisa(tabela, pos, &x, arq, &comparacoes, &transferencias, &tempo2);
    } else if(situacao == 2) {
        if ((arq = fopen("decrescente.bin","rb")) == NULL) {
            printf("Erro ao abrir arquivo"); 
            return;
        }
        pos = preprocessamentoDecrescente(tabela, arq, qtd, &transferencias, &tempo1, parametro);
        resultado = pesquisaDecrescente(tabela, pos, &x, arq, &comparacoes, &transferencias, &tempo2);
    }

    if(resultado) {
        printf("\nRegistro encontrado!\n");
        printf("Item: %d | %ld, %s \n", x.chave, x.dado1, x.dado2);
    } else {
        printf("\nRegistro nao foi encontrado!\n");
    }        

    printf(ANSI_COLOR_BLUE "\nCOMPARACOES             : %d" ANSI_COLOR_RESET, comparacoes);
    printf(ANSI_COLOR_BLUE "\nTRANSFERENCIAS          : %d" ANSI_COLOR_RESET, transferencias);
    printf(ANSI_COLOR_BLUE "\nTEMPO PREPROCESSAMENTO  : %lf segundos" ANSI_COLOR_RESET, tempo1);
    printf(ANSI_COLOR_BLUE "\nTEMPO EXECUCAO PESQUISA : %lf segundos" ANSI_COLOR_RESET, tempo2);

    fclose(arq);
}

// Gerando a tabela de índice das páginas
int preprocessamento(int tabela[], FILE *arq, int quantidade, int *transferencias, double *tempo, bool p) {
    // Uma página nada mais é que um conjunto de 4 items
    Item pag[ITENSPAGINA];
    int pos = 0, cont = 0;

    // Inicializamos as variáveis de auxílio a captar o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // A leitura dos itens é feita de 4 em 4 (tamanho de nossa página)
    while((fread(pag, ITENSPAGINA * sizeof(Item), 1, arq)) == 1) {
        cont++;
        if(cont > quantidade) break;
        // Inserimos na tabela de índices a primeira chave das 4 que lemos
        tabela[pos] = pag[0].chave;
        pos++;
    }

    // O número de leituras no arquivo de entrada é o nro de transferências entre memória secundária e principal
    *transferencias = cont;

    // Clock é parado e o tempo de execução é retornado em double
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    *tempo = cpu_time_used;
    
    // Imprimindo tabela de índices
    if(p) {
        int i;
        for(i = 0; i < quantidade / ITENSPAGINA; i++) {
            printf("\n%2d | %-8d", i, tabela[i]);
        }
        printf("\n");
    }
    
    fflush (stdout);
    return pos;
}

// Efetuando a pesquisa sequencial indexada
int pesquisa (int tab[], int tam, Item* item, FILE *arq, int *comparacoes, int *transferencias, double *tempo) {
    Item pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;
    *comparacoes = 0;

    // Inicializamos as variáveis de auxílio a captar o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Procura pela página onde o item pode se encontrar
    i = 0;
    while (i < tam && tab[i] <= item->chave) {
        i++;
        *comparacoes = *comparacoes + 1;
    }

    // Caso a chave desejada seja menor que a 1a chave, o item não existe no arquivo
    if (i == 0) return 0;

    // Se não, i = ?
    else {
        if (i < tam) quantitens = ITENSPAGINA;
        else {
            fseek (arq, 0, SEEK_END);
            quantitens = (ftell(arq)/sizeof(Item))%ITENSPAGINA;
            if(quantitens == 0) quantitens = ITENSPAGINA;
        }

        // Lê a página desejada do arquivo
        // desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(Item);
        desloc = (i-1)*ITENSPAGINA*sizeof(Item); 

        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Item), quantitens, arq);

        // O fread é executado mais uma vez, então mais uma nova transferência
        *transferencias = *transferencias + 1;
        
        // Pesquisa sequencial na página lida
        for (i=0; i < quantitens; i++) {
            *comparacoes = *comparacoes + 1;
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];
                // printf("Encontrado %d | %d", item->chave, item ->dado1);

                // Clock é parado e o tempo de execução é retornado em double
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                *tempo = cpu_time_used;
                return 1;
            }
        }

        // Clock é parado e o tempo de execução é retornado em double
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        *tempo = cpu_time_used;

        // Se não for encontrado o item, o retorno é de 0
        return 0;
    }
}

// Tabela ordenada decrescentemente
int preprocessamentoDecrescente(int tabela[], FILE *arq, int quantidade, int *transferencias, double *tempo, bool p) {
    Item pag[ITENSPAGINA];
    int pos = 0, cont = 0;

    // Inicializamos as variáveis de auxílio a captar o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // NAS ULTIMAS 200 POSICOES
    // fseek(arq, - (200 * sizeof x), SEEK_END);

    // A leitura dos itens é feita de 4 em 4 (tamanho de nossa página)
    while ((fread(pag, ITENSPAGINA * sizeof(Item), 1, arq)) == 1) {
        cont++;
        if(cont > quantidade) break;
        if (cont % ITENSPAGINA == 1) {
            tabela[pos] = pag[0].chave;
            // tabela[pos].posicao = pos + 1;
            pos++;
        }
    }

    // O número de leituras no arquivo de entrada é o nro de transferências entre memória secundária e principal
    *transferencias = cont;

    // Clock é parado e o tempo de execução é retornado em double
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    *tempo = cpu_time_used;
    
    // Imprimir tabela de índices
    if(p) {
        int i;
        for(i = 0; i < quantidade / ITENSPAGINA; i++) {
            printf("\n%2d | %-8d", i, tabela[i]);
        }
    }

    fflush (stdout);
    return pos;
}

// Pesquisa de maior para menor
int pesquisaDecrescente (int tab[], int tam, Item* item, FILE *arq, int *comparacoes, int *transferencias, double *tempo) {
    Item pagina[ITENSPAGINA];
    int i = 0, quantitens;
    long desloc;
    *comparacoes = 0;
    int nropag;

    // Inicializamos as variáveis de auxílio a captar o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Procura pela página onde o item pode se encontrar
    i = 0;
    while (i < tam && tab[i] >= item->chave) {
        i++;
        // printf("\nTabela[i].chave: %d", tab[i].chave);
        *comparacoes = *comparacoes + 1;
    }

    nropag = i;

    // Caso a chave desejada seja menor que a 1a chave, o item não existe no arquivo
    if (i == 0) return 0;

    // Se não, i = ?
    else {
        if (i < tam) quantitens = ITENSPAGINA;
        else {
            fseek (arq, 0, SEEK_END);
            quantitens = (ftell(arq)/sizeof(Item))%ITENSPAGINA;
            if(quantitens == 0) quantitens = ITENSPAGINA;
        }

        // Lê a página desejada do arquivo
        desloc = (i-1)*ITENSPAGINA*sizeof(Item);

        // comecando nas ultimas 200 posicoes
        // fseek(arq, - (200 * sizeof(Item)), SEEK_END);
        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Item), quantitens, arq);
        *transferencias = *transferencias + 1;
        
        // Pesquisa sequencial na página lida
        for (i = 0; i < quantitens; i++) {
            *comparacoes = *comparacoes + 1;
            // printf("\n\nPAGINA DE POSICAO %d | Chave: %d", nropag + 1, pagina[i].chave);
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];
                // printf("Encontrado %d | %d", item->chave, item ->dado1);

                // Clock é parado e o tempo de execução é retornado em double
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                *tempo = cpu_time_used;
                return 1;
            }
        }

        // Clock é parado e o tempo de execução é retornado em double
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        *tempo = cpu_time_used;
        
        // Se não for encontrado o item, o retorno é de 0
        return 0;
    }
}

// Test de 10 chaves
// int teste(FILE *arq) {
//     int pos, qtd; int comparacoes = 0, transferencias = 0; double tempo;
//     int resultados[10]; int chaves[10] = {1,2,3,400,500,6,7,8,9000,10}; 

//     int i, j = 100, k;
//     for(i = 0; i < 6; i++) {
//         tipoindice tabela[MAXTABELA]; Item x;
//         pos = preprocessamento(tabela, arq, j, &transferencias, &tempo);
//         for(k = 0; k < 10; k++) {
//             x.chave = chaves[k] + 4*i + k*2;
//             printf("\nChave: %d", x.chave);
//             resultados[k] = pesquisa(tabela, pos, &x, arq, &comparacoes, &transferencias, &tempo);

//             if(resultados[k]) {
//                 printf("\nRegistro encontrado!");
//                 // printf("Item: %d | %ld, %s \n", x.chave, x.dado1, x.dado2);
//             } else {
//                 printf("\nRegistro nao foi encontrado :(");
//             }
//             rewind(arq);
//         }
//         printf(ANSI_COLOR_BLUE "\nCOMPARACOES EM ARQ DE %d REGISTROS       : %d" ANSI_COLOR_RESET, j, comparacoes);
//         j = j * 10;
//     }

//     return 0;
// }