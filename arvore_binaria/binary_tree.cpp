#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

typedef struct{
    int key;
    long int data1;
    char data2[501];
}Register;

typedef struct{
    Register reg;
    int right;
    int left;
}Node;

Node create_node(Node a);
void insert(FILE* output_file,FILE* input_file, long long*transf ,  long long*comp);
int insert_right_node(Node node, Node aux, int count, int* position, FILE** file);
int insert_left_node( Node node, Node aux, int count, int* position, FILE** file);
int open_binary_file(FILE** file);
int search_node(int key, FILE** file);

int main(){
    long long comp=0, transf=0;
    FILE* output_file;
    FILE* input_file = fopen("crescente.bin","rb");
    //caso o arquivo abra com sucesso, executa:
     if(open_binary_file( &output_file )){
        insert(output_file, input_file, &transf, &comp);
        //Volta para o início do arquivo
        cout<<"Quantidade de comparações: "<<comp<<endl;
        cout<<"Quantidade de transferencias: "<<transf<<endl;
        rewind(output_file);
        //realiza a pesquisa do item 18
        if(search_node( 91, &output_file )){
            cout<<"item encontrado"<<endl;
        }
        else{
            cout<<"item não encontrado"<<endl;
        }
    } 
                     
    return 0;
}

void insert(FILE* output_file, FILE* input_file,  long long*transf ,  long long*comp){
    Node a, aux;
    int i = -1;

    
    (*transf)++;
    while(fread(&a.reg,sizeof(Register),1,input_file)== 1){
        i++;

        a= create_node(a);

         //Caso o arquivo esteja vazio
         if( ftell( output_file) - 1 < 0){
            fwrite( &a , sizeof(Node), 1, output_file );
        }
        else{
            int inserted = 0;
            int position = 0; 
            //Enquando o registro não for inserido
            while ( inserted == 0 )
            {   //Caminha no arquivo de acordo com a posição selecionada
                fseek( output_file, position*sizeof(Node), SEEK_SET);
                //Caso seja possivel ler um item, ele é lido
                (*transf)++;
                if(fread( &aux, sizeof(Node), 1, output_file ) == 1){
                    //Caso a chave seja maior do que do item lido, é feita a tentativa de inserir à direita
                    
                    if(a.reg.key > aux.reg.key){
                        (*comp)++;
                        inserted = insert_right_node( a, aux, i, &position, &output_file );
                    }
                    //Caso a chave seja menor do que do item lido, é feita a tentativa de inserir à esquerda
                    else if(a.reg.key < aux.reg.key){
                        (*comp)++;
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
}

Node create_node(Node a ){
    a.right = -1;
    a.left = -1;
    return a;
}

int open_binary_file( FILE** file ){
 if ((*file = fopen("arvore.bin", "w+b")) == NULL)
    {
        cout << "Erro na abertura do file\n";
        return 0;
    }
 return 1;
}

int insert_right_node( Node node, Node aux, int count, int* position, FILE** file ){
    if (aux.left == -1){
        aux.left = count;
        fseek( *file, (*position * sizeof(Node)), SEEK_SET);
        fwrite( &aux, sizeof(Node), 1, *file);
        fseek( *file, (count* sizeof(Node)), SEEK_SET);
        fwrite( &node, sizeof(Node), 1, *file);
        return 1;
    }
    *position = aux.left;
    return 0;   
}

int insert_left_node( Node node, Node aux, int count, int* position, FILE** file ){
    if (aux.right == -1){
        aux.right = count;
        fseek( *file, (*position * sizeof(Node)), SEEK_SET);
        fwrite( &aux, sizeof(Node), 1, *file);
        fseek( *file, (count* sizeof(Node)), SEEK_SET);
        fwrite( &node, sizeof(Node), 1, *file);
        return 1;
    }
    *position = aux.right;
    return 0;   
}

int search_node( int key, FILE** file ){
    Node aux;
    int position = 0;
    while ( fread(&aux, sizeof( Node), 1, *file) == 1 )
    {
        fseek( *file, position*sizeof(Node), SEEK_SET);
        if(fread( &aux, sizeof(Node), 1, *file ) == 1){
            
            if(key > aux.reg.key){
                position = aux.left;
            }
            else if(key < aux.reg.key){
                position = aux.right;
            }
            else{
                return 1;
            }
        } 
    }  
    return 0;
}