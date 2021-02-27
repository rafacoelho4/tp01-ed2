#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "./sequencial_indexado/index.cpp"
#include "./arvore_binaria/binary_tree.cpp"
#include "./gerador_de_arquivos/arquivo.cpp"
#include "./arvore_b_estrela/b_star.cpp"
#include "./arvore_b/b_tree.cpp"

#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int main(int argc, char *argv[]) 
{
    FILE *arq;
    tipoindice tabela[MAXTABELA];
    int comparacoes = 0, tranferencias = 0;

    clock_t start, end;
    double cpu_time_used;

    int metodo = atoi(argv[1]);
    int qtd = atoi(argv[2]);
    int situacao = atoi(argv[3]);
    int chave = atoi(argv[4]);
    
    bool parametro = false;
    if( argc > 5){
        string p = argv[5];
        if(p == "-P"){
         parametro = true;
        }
        else{
            cout<<"Parametro invalido\n";
            exit(1);
        }
    }

    // argv deve ser:
    // pesquisa <metodo> <quantidade> <situação> <chave> [-P]
    // [0]      [1]      [2]          [3]        [4]     [5]

    // <método> representa o método de pesquisa externa a ser executado, podendo ser um número
    // inteiro de 1 a 4, de acordo com a ordem dos métodos mencionados;

    // <quantidade> representa a quantidade de registros do arquivo considerado;

    // <situação> representa a situação de ordem do arquivo, podendo ser 1 (arquivo ordenado
    // ascendentemente), 2 (arquivo ordenado descendentemente) ou 3 (arquivo desordenado
    // aleatoriamente);

    // <chave> representa a chave a ser pesquisada no arquivo considerado;

    // [-P] representa um argumento opcional que deve ser colocado quando se deseja que as chaves de
    // pesquisa dos registros do arquivo considerado sejam apresentadas na tela. 

    if (metodo == 1) {
        // acesso sequencial indexado
        int pos; int resultado;
        Item x;
        x.chave = chave;

        if(situacao == 3) {
            printf("\nNao e possivel executar uma pesquisa sequencial indexada em um arquivo aleatorio.");
            return 0;
        } else if(situacao == 1) {

            start = clock();

            if ((arq = fopen("./entrada/crescente.bin","rb")) == NULL) {
                printf("Erro ao abrir arquivo"); 
                return 0;
            }

             // Retorna pos = tamanho da tabela de indice de paginas
            pos = preprocessamento(tabela, arq, qtd);

            resultado = pesquisa(tabela, pos, &x, arq, &comparacoes);

            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

        } else if(situacao == 2) {

            start = clock();

            if ((arq = fopen("./entrada/decrescente.bin","rb")) == NULL) {
                printf("Erro ao abrir arquivo"); 
                return 0;
            }

             // Retorna pos = tamanho da tabela de indice de paginas
            pos = preprocessamentoDecrescente(tabela, arq, qtd);

            resultado = pesquisaDecrescente(tabela, pos, &x, arq, &comparacoes);

            end = clock();
            cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        }

        if(resultado) {
            printf("\nRegistro encontrado!");
            printf("Item: %d | %ld, %s \n", x.chave, x.dado1, x.dado2);
        } else {
            printf("\nRegistro não foi encontrado!");
        }        

        printf(ANSI_COLOR_BLUE "\nCOMPARACOES       : %d" ANSI_COLOR_RESET, comparacoes);
        printf(ANSI_COLOR_BLUE "\nTEMPO DE EXECUCAO : %lf segundos" ANSI_COLOR_RESET, cpu_time_used);

        fclose(arq);

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