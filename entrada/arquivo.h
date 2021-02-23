#ifndef ARQUIVO_H
#define ARQUIVO_H

typedef struct {
    int chave;
    long int dado1;
    char dado2[501];
} Item;

void rand_str(char *dest, size_t length);

void criarArquivoCrescente ();

void criarArquivoDecrescente ();

void criarArquivoAleatorio ();

#endif