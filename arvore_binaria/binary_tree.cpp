#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef struct{
    int key;
    int right;
    int left;
}Register;

Register create_register(int key);
int insert_right_node(Register node, Register aux, int count, int* position, FILE** file);
int insert_left_node( Register node, Register aux, int count, int* position, FILE** file);
int open_binary_file(FILE** file);
int search_register(int key, FILE** file);

int main(){
    FILE* output_file;
    Register a, aux;
    int sort;
    //caso o arquivo abra com sucesso, executa:
    if(open_binary_file( &output_file )){
        //Realiza o sorteio de elementos para a arvore
        for( int i = 0; i < 10; i ++){
            if( i % 2 == 0)
                sort = i + 10;
            else sort = i + 8;
            //função que cria um novo registro e inicializa os apontadores com -1
            a = create_register( sort );
            //Caso o arquivo esteja vazio
            if( ftell( output_file) - 1 < 0){
                fwrite( &a , sizeof(Register), 1, output_file );
            }
            else{
                int inserted = 0;
                int position = 0;
                //Enquando o registro não for inserido
                while ( inserted == 0 )
                {   //Caminha no arquivo de acordo com a posição selecionada
                    fseek( output_file, position*sizeof(Register), SEEK_SET);
                    //Caso seja possivel ler um item, ele é lido
                    if(fread( &aux, sizeof(Register), 1, output_file ) == 1){
                       //Caso a chave seja maior do que do item lido, é feita a tentativa de inserir à direita
                        if(a.key > aux.key){
                            inserted = insert_right_node( a, aux, i, &position, &output_file );
                        }
                        //Caso a chave seja menor do que do item lido, é feita a tentativa de inserir à esquerda
                        else if(a.key < aux.key){
                            inserted = insert_left_node( a, aux, i, &position, &output_file );
                        }
                        //Caso o item seja igual, é feita a finalização do laço
                        else{
                            inserted = 1;
                        }
                    }
                }
            }      
        }
        //Volta para o início do arquivo
        rewind(output_file);
        //realiza a pesquisa do item 18
        if(search_register( 18, &output_file )){
            cout<<"item encontrado"<<endl;
        }
        else{
            cout<<"item não encontrado"<<endl;
        }
    }
                     
    return 0;
}

Register create_register( int key ){
    Register r;

    r.right = -1;
    r.left = -1;
    r.key = key;

    return r;
}

int open_binary_file( FILE** file ){
 if ((*file = fopen("arvore.bin", "w+b")) == NULL)
    {
        cout << "Erro na abertura do file\n";
        return 0;
    }
 return 1;
}

int insert_right_node( Register node, Register aux, int count, int* position, FILE** file ){
    if (aux.left == -1){
        aux.left = count;
        fseek( *file, (*position * sizeof(Register)), SEEK_SET);
        fwrite( &aux, sizeof(Register), 1, *file);
         fseek( *file, (count* sizeof(Register)), SEEK_SET);
        fwrite( &node, sizeof(Register), 1, *file);
        return 1;
    }
    *position = aux.left;
    return 0;   
}

int insert_left_node( Register node, Register aux, int count, int* position, FILE** file ){
    if (aux.right == -1){
        aux.right = count;
        fseek( *file, (*position * sizeof(Register)), SEEK_SET);
        fwrite( &aux, sizeof(Register), 1, *file);
        fseek( *file, (count* sizeof(Register)), SEEK_SET);
        fwrite( &node, sizeof(Register), 1, *file);
        return 1;
    }
    *position = aux.right;
    return 0;   
}

int search_register( int key, FILE** file ){
    Register aux;
    int position = 0;
    while ( fread(&aux, sizeof( Register), 1, *file) == 1 )
    {
        fseek( *file, position*sizeof(Register), SEEK_SET);
        if(fread( &aux, sizeof(Register), 1, *file ) == 1){
            
            if(key > aux.key){
                position = aux.left;
            }
            else if(key < aux.key){
                position = aux.right;
            }
            else{
                return 1;
            }
        } 
    }  
    return 0;
}