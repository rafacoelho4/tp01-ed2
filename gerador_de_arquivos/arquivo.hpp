#ifndef ARQUIVO_H
#define ARQUIVO_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <algorithm> // Para o shuffle()
#include <chrono>

void rand_str(char *dest, size_t length);

void criarArquivoCrescente (int);

void criarArquivoDecrescente (int);

void criarArquivoAleatorio (int);

int* criaChaves (int);

#endif