#include <stdio.h>
#include <time.h>
#include "index.hpp"

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void sequencial_indexado(int situacao, int chave, int qtd, int parametro) {
    FILE *arq;
    tipoindice tabela[MAXTABELA];
    int comparacoes = 0, tranferencias = 0; double tempo1, tempo2;

    int pos; int resultado;
    Item x; x.chave = chave;

    if(situacao == 1) {
        if ((arq = fopen("crescente.bin","rb")) == NULL) {
            printf("Erro ao abrir arquivo"); 
            return;
        }
        pos = preprocessamento(tabela, arq, qtd, &tempo1);
        resultado = pesquisa(tabela, pos, &x, arq, &comparacoes, &tempo2);
    } else if(situacao == 2) {
        if ((arq = fopen("decrescente.bin","rb")) == NULL) {
            printf("Erro ao abrir arquivo"); 
            return;
        }
        pos = preprocessamentoDecrescente(tabela, arq, qtd, &tempo1);
        resultado = pesquisaDecrescente(tabela, pos, &x, arq, &comparacoes, &tempo2);
    }

    if(resultado) {
        printf("\nRegistro encontrado!");
        printf("Item: %d | %ld, %s \n", x.chave, x.dado1, x.dado2);
    } else {
        printf("\nRegistro não foi encontrado!");
    }        

    printf(ANSI_COLOR_BLUE "\nCOMPARACOES             : %d" ANSI_COLOR_RESET, comparacoes);
    printf(ANSI_COLOR_BLUE "\nTEMPO PREPROCESSAMENTO  : %lf segundos" ANSI_COLOR_RESET, tempo1);
    printf(ANSI_COLOR_BLUE "\nTEMPO EXECUCAO PESQUISA : %lf segundos" ANSI_COLOR_RESET, tempo2);

    fclose(arq);
}

// Gerando a tabela de índice das páginas
// Retorna pos = tamanho da tabela de indice de paginas
int preprocessamento(tipoindice tabela[], FILE *arq, int quantidade, double *tempo) {
    Item x;
    int pos = 0, cont = 0;

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    while ((fread(&x, sizeof(x), 1, arq)) == 1) {
        cont++;
        if(cont > quantidade) break;
        if (cont % ITENSPAGINA == 1) {
            tabela[pos].chave = x.chave;
            tabela[pos].posicao = pos + 1;
            pos++;
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    *tempo = cpu_time_used;
    
    // Imprimindo tabela de índices
    // int i;
    // for(i = 0; i < quantidade / ITENSPAGINA; i++) {
    //     printf("\n%2d | %2d | %-8d", i, tabela[i].posicao, tabela[i].chave);
    // }

    fflush (stdout);
    return pos;
}

// Efetuando a pesquisa sequencial indexada
int pesquisa (tipoindice tab[], int tam, Item* item, FILE *arq, int *comparacoes, double *tempo) {
    Item pagina[ITENSPAGINA];
    int i, quantitens;
    long desloc;
    *comparacoes = 0;

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Procura pela página onde o item pode se encontrar
    i = 0;
    while (i < tam && tab[i].chave <= item->chave) {
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
        desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(Item);

        fseek (arq, desloc, SEEK_SET);
        fread (&pagina, sizeof(Item), quantitens, arq);
        
        // Pesquisa sequencial na página lida
        for (i=0; i < quantitens; i++) {
            *comparacoes = *comparacoes + 1;
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];
                // printf("Encontrado %d | %d", item->chave, item ->dado1);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                *tempo = cpu_time_used;
                return 1;
            }
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        *tempo = cpu_time_used;
        return 0;
    }
}

// Tabela ordenada decrescentemente
int preprocessamentoDecrescente(tipoindice tabela[], FILE *arq, int quantidade, double *tempo) {
    Item x;
    int pos = 0, cont = 0;

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // NAS ULTIMAS 200 POSICOES
    // fseek(arq, - (200 * sizeof x), SEEK_END);

    while ((fread(&x, sizeof(x), 1, arq)) == 1) {
        cont++;
        if(cont > quantidade) break;
        if (cont % ITENSPAGINA == 1) {
            tabela[pos].chave = x.chave;
            tabela[pos].posicao = pos + 1;
            pos++;
        }
    }

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    *tempo = cpu_time_used;
    
    // Imprimir tabela de índices
    // int i;
    // for(i = 0; i < quantidade / ITENSPAGINA; i++) {
    //     printf("\n%2d | %2d | %-8d", i, tabela[i].posicao, tabela[i].chave);
    // }

    fflush (stdout);
    return pos;
}

// Pesquisa de maior para menor
int pesquisaDecrescente (tipoindice tab[], int tam, Item* item, FILE *arq, int *comparacoes, double *tempo) {
    Item pagina[ITENSPAGINA];
    int i = 0, quantitens;
    long desloc;
    *comparacoes = 0;
    int nropag;

    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Procura pela página onde o item pode se encontrar
    i = 0;
    while (i < tam && tab[i].chave >= item->chave) {
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
        desloc = (tab[i-1].posicao-1)*ITENSPAGINA*sizeof(Item);

        // comecando nas ultimas 200 posicoes
        // fseek(arq, - (200 * sizeof(Item)), SEEK_END);
        fseek (arq, desloc, SEEK_SET);
        
        fread (&pagina, sizeof(Item), quantitens, arq);
        
        // Pesquisa sequencial na página lida
        for (i = 0; i < quantitens; i++) {
            *comparacoes = *comparacoes + 1;
            // printf("\n\nPAGINA DE POSICAO %d | Chave: %d", nropag + 1, pagina[i].chave);
            if (pagina[i].chave == item->chave) {
                *item = pagina[i];
                // printf("Encontrado %d | %d", item->chave, item ->dado1);
                end = clock();
                cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
                *tempo = cpu_time_used;
                return 1;
            }
        }
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        *tempo = cpu_time_used;
        return 0;
    }
}

// Test de 10 chaves
int teste(FILE *arq) {
    int pos, qtd; int comparacoes = 0, transferencias = 0; double tempo;
    int resultados[10]; int chaves[10] = {1,2,3,400,500,6,7,8,9000,10}; 

    int i, j = 100, k;
    for(i = 0; i < 6; i++) {
        tipoindice tabela[MAXTABELA]; Item x;
        pos = preprocessamento(tabela, arq, j, &tempo);
        for(k = 0; k < 10; k++) {
            x.chave = chaves[k] + 4*i + k*2;
            printf("\nChave: %d", x.chave);
            resultados[k] = pesquisa(tabela, pos, &x, arq, &comparacoes, &tempo);

            if(resultados[k]) {
                printf("\nRegistro encontrado!");
                // printf("Item: %d | %ld, %s \n", x.chave, x.dado1, x.dado2);
            } else {
                printf("\nRegistro nao foi encontrado :(");
            }
            rewind(arq);
        }
        printf(ANSI_COLOR_BLUE "\nCOMPARACOES EM ARQ DE %d REGISTROS       : %d" ANSI_COLOR_RESET, j, comparacoes);
        j = j * 10;
    }

    return 0;
}