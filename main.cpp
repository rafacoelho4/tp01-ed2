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
            sequencial_indexado(situacao, chave, qtd, parametro);
        } else if (situacao == 2) {
            criarArquivoDecrescente(qtd);
            sequencial_indexado(situacao, chave, qtd, parametro);
        }
    }
    
    if (metodo == 2) {
        if(situacao == 1 ){
            criarArquivoCrescente(qtd);
            binary_tree(chave,"crescente.bin",parametro);
        }
        else if(situacao == 2 ){
            criarArquivoDecrescente(qtd);
            binary_tree(chave,"decrescente.bin",parametro);
        }
        else{
            criarArquivoAleatorio(qtd);
            binary_tree(chave,"aleatorio.bin",parametro);
        }
    }

     if (metodo == 3) {
        if(situacao == 1 ){
            criarArquivoCrescente(qtd);
            b_tree(chave,"crescente.bin",parametro);
        }
        else if(situacao == 2 ){
            criarArquivoDecrescente(qtd);
             b_tree(chave,"decrescente.bin",parametro);
            
        }
        else{
            criarArquivoAleatorio(qtd);
             b_tree(chave,"aleatorio.bin",parametro);
            
        }
    }
    
       if (metodo == 4) { 
        cout<<"oi:";
   
        if(situacao == 1 ){
            criarArquivoCrescente(qtd);
            b_star(chave, "crescente.bin", qtd, parametro);
            
           
            // TODO: fclose(arq); aqui?
        }
        else if(situacao == 2 ){
            criarArquivoDecrescente(qtd);
             b_star(chave, "decrescente.bin", qtd, parametro);
          
        }
        else{
            criarArquivoAleatorio(qtd);
             b_star(chave, "aleatorio.bin", qtd, parametro);

        } 
    } 

    return 0;
}