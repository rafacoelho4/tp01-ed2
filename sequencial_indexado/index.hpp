#ifndef INDEX_H
#define INDEX_H

#include <stdio.h>
#include "../tipos.hpp"

#define ITENSPAGINA 4
#define MAXTABELA 100000

// Definição da tabela de índice das páginas
typedef struct {
    int posicao;
    int chave;
} tipoindice;

int preprocessamento(tipoindice tabela[], FILE *arq, int quantidade, int *transferencias, double *tempo);

int pesquisa (tipoindice tab[], int tam, Item* item, FILE *arq, int *comparacoes, int *transferencias, double *tempo);

int preprocessamentoDecrescente(tipoindice tabela[], FILE *arq, int quantidade, int *transferencias, double *tempo);

int pesquisaDecrescente (tipoindice tab[], int tam, Item* item, FILE *arq, int *comparacoes, int *transferencias, double *tempo);

#endif