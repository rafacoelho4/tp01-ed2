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
        pos = preprocessamento(tabela, arq, qtd, &transferencias, &tempo1);
        resultado = pesquisa(tabela, pos, &x, arq, &comparacoes, &transferencias, &tempo2, parametro);
    } else if(situacao == 2) {
        if ((arq = fopen("decrescente.bin","rb")) == NULL) {
            printf("Erro ao abrir arquivo"); 
            return;
        }
        pos = preprocessamentoDecrescente(tabela, arq, qtd, &transferencias, &tempo1);
        resultado = pesquisaDecrescente(tabela, pos, &x, arq, &comparacoes, &transferencias, &tempo2, parametro);
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
int preprocessamento(int tabela[], FILE *arq, int quantidade, int *transferencias, double *tempo) {
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
    // int i;
    // for(i = 0; i < quantidade / ITENSPAGINA; i++) {
    //     printf("\n%2d | %-8d", i, tabela[i]);
    // }
    // printf("\n");
    
    
    fflush (stdout);
    return pos;
}

// Efetuando a pesquisa sequencial indexada
int pesquisa (int tab[], int tam, Item* item, FILE *arq, int *comparacoes, int *transferencias, double *tempo, bool p) {
    Item pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;
    *comparacoes = 0;

    // Inicializamos as variáveis de auxílio a captar o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    if(p) {
        // Procura pela página onde o item pode se encontrar
        i = 0;
        while (i < tam && tab[i] <= item->chave) {
            printf("\nComparacao na tabela: %d", tab[i]);
            i++;
            *comparacoes = *comparacoes + 1;
        }
        printf("\n\n");
    } else {
        // Procura pela página onde o item pode se encontrar
        i = 0;
        while (i < tam && tab[i] <= item->chave) {
            i++;
            *comparacoes = *comparacoes + 1;
        }
    }

    // Caso a chave desejada seja menor que a 1a chave, o item não existe no arquivo
    if (i == 0) return 0;

    // Se não, i = ?
    else {
        // Se eu nao tiver na última página, minha página está cheia com ITENSPAGINA itens
        if (i < tam) quantitens = ITENSPAGINA;
        // Se eu tiver na última página, verifico se ela está cheia ou não
        else {
            // Vou até o final do arquivo
            fseek (arq, 0, SEEK_END);
            // Posição final do arquivo dividio pelo tamanho de itens = nro de registros totais 
            // Nro de registros módulo de ITENSPAGINA = quantidade de itens na minha ultima pagina
            quantitens = (ftell(arq)/sizeof(Item))%ITENSPAGINA;
            // Se for igual a 0, página esta cheia
            if(quantitens == 0) quantitens = ITENSPAGINA;
        }

        // Lê a página desejada do arquivo
        desloc = (i-1)*ITENSPAGINA*sizeof(Item); 

        // Vou até a pagina que contem meu item
        fseek (arq, desloc, SEEK_SET);

        // Leio a pagina inteira
        fread (&pagina, sizeof(Item), quantitens, arq);

        // O fread é executado mais uma vez, então mais uma nova transferência
        *transferencias = *transferencias + 1;
        
        // Pesquisa sequencial na página lida
        for (i=0; i < quantitens; i++) {
            *comparacoes = *comparacoes + 1;
            if(p){
                printf("Comparacao na pagina: %d\n", pagina[i].chave);
            }
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];

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
int preprocessamentoDecrescente(int tabela[], FILE *arq, int quantidade, int *transferencias, double *tempo) {
    // Uma página nada mais é que um conjunto de 4 items
    Item pag[ITENSPAGINA];
    int pos = 0, cont = 0;

    // Inicializamos as variáveis de auxílio a captar o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // A leitura dos itens é feita de 4 em 4 (tamanho de nossa pagina)
    while ((fread(pag, ITENSPAGINA * sizeof(Item), 1, arq)) == 1) {
        cont++;
        if(cont > quantidade) break;
        tabela[pos] = pag[0].chave;
        pos++;
    }

    // O número de leituras no arquivo de entrada é o nro de transferências entre memória secundária e principal
    *transferencias = cont;

    // Clock é parado e o tempo de execução é retornado em double
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    *tempo = cpu_time_used;
    
    // Imprimir tabela de índices
    // int i;
    // for(i = 0; i < quantidade / ITENSPAGINA; i++) {
    //     printf("\n%2d | %-8d", i, tabela[i]);
    // }
    
    fflush (stdout);
    return pos;
}

// Pesquisa de maior para menor
int pesquisaDecrescente (int tab[], int tam, Item* item, FILE *arq, int *comparacoes, int *transferencias, double *tempo, bool p) {
    Item pagina[ITENSPAGINA];
    int i = 0, quantitens;
    long desloc;
    *comparacoes = 0;

    // Inicializamos as variáveis de auxílio a captar o tempo de execução
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Procura pela página onde o item pode se encontrar
    if(p) { 
        i = 0;
        while (i < tam && tab[i] >= item->chave) {
            printf("\nComparacao na tabela: %d", tab[i]);
            i++;
            *comparacoes = *comparacoes + 1;
        }
        printf("\n\n");
    } else {
        i = 0;
        while (i < tam && tab[i] >= item->chave) {
            i++;
            *comparacoes = *comparacoes + 1;
        }
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
        desloc = (i-1)*ITENSPAGINA*sizeof(Item);

        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Item), quantitens, arq);
        *transferencias = *transferencias + 1;
        
        // Pesquisa sequencial na página lida
        for (i = 0; i < quantitens; i++) {
            *comparacoes = *comparacoes + 1;
            if(p){
                printf("Comparacao na pagina: %d\n", pagina[i].chave);
            }
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];

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