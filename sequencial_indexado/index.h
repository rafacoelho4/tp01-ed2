#ifndef INDEX_H
#define INDEX_H

#include <stdio.h>
#include "../tipos.h"

#define ITENSPAGINA 4
#define MAXTABELA 100000

// Definição da tabela de índice das páginas
typedef struct {
    int posicao;
    int chave;
} tipoindice;

int preprocessamento(tipoindice tabela[], FILE *arq, int quantidade);

int pesquisa (tipoindice tab[], int tam, Item* item, FILE *arq, int *comparacoes);

int preprocessamentoDecrescente(tipoindice tabela[], FILE *arq, int quantidade);

int pesquisaDecrescente (tipoindice tab[], int tam, Item* item, FILE *arq, int *comparacoes);

#endif