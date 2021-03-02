#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "./sequencial_indexado/index.cpp"
#include "./arvore_binaria/binary_tree.cpp"
#include "./gerador_de_arquivos/arquivo.cpp"
#include "./arvore_b_estrela/b_star.cpp"
#include "./arvore_b/b_tree.cpp"

int main(int argc, char *argv[]) 
{
    int metodo = atoi(argv[1]);
    int qtd = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);

    int* chaves = (int*) malloc(qtd * sizeof(int));
    
    bool parametro = false;
    if(argc > 5){
        string p = argv[5];
        if(p == "-P"){
         parametro = true;
        }
        else{
            cout<<"Parametro invalido\n";
            exit(1);
        }
    }

    if (metodo == 1) {
        if(situacao == 3) {
            return 0;
        } else if (situacao == 1) {
            criarArquivoCrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                sequencial_indexado(situacao, chaves[i], qtd, parametro);
                printf("\n");
            }

            //sequencial_indexado(situacao, chave, qtd, parametro);
        } else if (situacao == 2) {
            criarArquivoDecrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                sequencial_indexado(situacao, chaves[i], qtd, parametro);
                printf("\n");
            }

            //sequencial_indexado(situacao, chave, qtd, parametro);
        }
    }
    
    if (metodo == 2) {
        if(situacao == 1 ){
            criarArquivoCrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                binary_tree(chaves[i],"crescente.bin", parametro);
                printf("\n");
            }

            //binary_tree(chave,"crescente.bin",parametro);
        }
        else if(situacao == 2 ){
            criarArquivoDecrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                binary_tree(chaves[i],"decrescente.bin", parametro);
                printf("\n");
            }

            //binary_tree(chave,"decrescente.bin",parametro);
        }
        else{
            criarArquivoAleatorio(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                binary_tree(chaves[i],"aleatorio.bin", parametro);
                printf("\n");
            }

            //binary_tree(chave,"aleatorio.bin",parametro);
        }
    }

     if (metodo == 3) {
        if(situacao == 1 ){
            criarArquivoCrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                b_tree(chave,"crescente.bin", parametro);
                printf("\n");
            }

            //b_tree(chave,"crescente.bin",parametro);
        }
        else if(situacao == 2 ){
            criarArquivoDecrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                b_tree(chaves[i], "decrescente.bin", parametro);
                printf("\n");
            }

            //b_tree(chave,"decrescente.bin",parametro);
            
        }
        else{
            criarArquivoAleatorio(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                b_tree(chaves[i],"aleatorio.bin", parametro);
                printf("\n");
            }

            //b_tree(chave,"aleatorio.bin",parametro);
            
        }
    }
    
       if (metodo == 4) { 
    
        if(situacao == 1 ){
            criarArquivoCrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                b_star(chaves[i], "crescente.bin", qtd, parametro);
                printf("\n");
            }

            //b_star(chave, "crescente.bin", qtd, parametro);
        }
        else if(situacao == 2 ){
            criarArquivoDecrescente(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                b_star(chaves[i], "crescente.bin", qtd, parametro);
                printf("\n");
            }
            
            //b_star(chave, "decrescente.bin", qtd, parametro);
        }
        else{
            criarArquivoAleatorio(qtd);

            // Teste automático com 10 chaves
            chaves = criaChaves(qtd);
            for(int i = 0; i < 10; i++){
                printf("\n%d TESTE:\n", i+1);
                b_star(chaves[i], "crescente.bin", qtd, parametro);
                printf("\n");
            }
            
            //b_star(chave, "aleatorio.bin", qtd, parametro);
        } 
    } 

    return 0;
}