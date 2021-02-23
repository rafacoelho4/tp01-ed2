#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define M 2
typedef struct Node Node;
typedef struct Node Pointer;

typedef struct{
    int key;
    int data[100];
}Register;


struct Node{
    int n;
    Register r[(M*M)];
    Pointer* p[(M*M)+1];
};



void print(Pointer* tree);
void insert(Register reg, Pointer** tree);
bool search(Register* reg, Pointer* tree);
void insert_on_node(Pointer** tree, Register* reg, Pointer** right);
void find_to_insert(Register reg, Pointer** tree, bool* grew, Register* rr, Pointer** pr);

int main(){
    Pointer* tree =(Pointer*) malloc( sizeof(Node));
    tree = NULL;
    Register r;

    int vet[23] = { 11, 36, 53, 95, 8, 91, 81, 48, 25, 27, 31, 16, 21, 59, 63, 72, 78, 14, 13, 12, 35, 33, 42};
     for( int i = 0; i < 23; i++){
        r.key= vet[i];
        insert( r, &tree );
    }   
        print(tree);
    
}

void print(Pointer* tree){
    int i = 0;

    if( tree == NULL ){
        return;
    }

    while ( i <= tree->n)
    {
        print(tree->p[i]);
        if(i!=tree->n){
            cout<<tree->r[i].key<<" ";
        }
        i++;
    }
    
    
}

bool search( Register* r, Pointer* tree ){
    long i = 1;

    //Caminha na página até encontrar o local desejado
    while(i < tree->n && r->key > tree->r[i-1].key) i++;

    //Compara o valor, caso seja igual : retorna verdadeiro e a pesquisa se encerra
     if(r->key == tree->r[i-1].key){
        *r = tree->r[i-1];
        return true;
    }
    
    //Caso o valor do registro seja menor que o ultimo registro da pagina: caminhamento à esquerda
    if(r->key < tree->r[i-1].key)
        search( r, tree->p[i-1]);
    
    //Caso contrário: caminhamento à direita
    search( r, tree->p[i]);
    
    //retorna falso caso não encontre o valor
    return false;
} 


void insert(Register reg, Pointer** tree){
   
    //Variável responsavel por verificar se a arvore cresceu
    bool grew;
    //Armazena o registro de retorno
    Register rr;
    //Armazena filho à direita do registro retornado
    Pointer *pr;
    //Ponteiro de páginas temporários
    Pointer* p_temp;
    //Realiza uma busca na arvore para inserir o registro reg
    find_to_insert(reg, tree, &grew, &rr, &pr);
     

    //Criação de um novo nó raiz
     if(grew){
        p_temp  =(Pointer*) malloc( sizeof(Node));
        p_temp->n = 1;
        p_temp->r[0] = rr;
        p_temp->p[1] = pr;
        p_temp->p[0] = (*tree);
        *tree = p_temp;
    } 
}

void find_to_insert(Register reg, Pointer** tree, bool *grew, Register* rr, Pointer** pr){
    //Variável que caminha pelas paginas
    long i = 1;
    //Variável responsável por definir local de divisão dos filhos à direita da página
    long j;
    //Ponteiro de páginas temporário
    Pointer* p_temp;
    
    //Verifica se arvore está vazia e retorna os valores para a criação da raiz
   if(*tree == NULL){
        *grew = true;
        *rr = reg;
        *pr = NULL;
        return; 
    }  

     //Laço que caminha pela pagina apontada pela raiz da arvore
    while(i < (*tree)->n && reg.key >(*tree)->r[i-1].key)  i++;
    
    //Verificação caso o registro já esteja presente na arvore
    if(reg.key ==(*tree)->r[i-1].key){
        cout<<"Erro: O registro já se encontra na arvore";
        *grew = false;
        return;
    }
    
    //A proxima procura será com o filho a esquerda caso a chave seja menor que o ultimo registro
    if(reg.key < (*tree)->r[i-1].key) i--;

    //Realiza a pesquisa na subarvore selecionada
    find_to_insert(reg, &((*tree)->p[i]), grew, rr, pr);

    //Verifica se será necessaria a criação de uma nova pagina
    if(!(*grew)) return;

    //Caso a pagina tenha espaço
    if( (*tree)->n < (M*M)){
        insert_on_node( tree, rr, pr);
        *grew = false;
        return;
    }

    //Caso a pagina precise ser dividida
    //Aloca ponteiro auxiliar e inicializa
    p_temp = (Pointer*) malloc( sizeof(Node) );
    p_temp->n = 0;
    p_temp->p[0] = NULL;

    if(i < (M+1)){
        insert_on_node(&p_temp,&(*tree)->r[(M*M)-1],&(*tree)->p[(M*M)]);
        (*tree)->n--;
        insert_on_node(tree, rr, pr);
    }

    else insert_on_node(&p_temp, rr, pr);

    //Processo de divisão das paginas
    for( j = M + 2; j <=(M*M); j++ ){
        insert_on_node(&p_temp,&(*tree)->r[j-1],&(*tree)->p[j]);
    }
    
   (*tree)->n = M;
    p_temp->p[0] = (*tree)->p[M+1];
    *rr = (*tree)->r[M];
    *pr = p_temp;
}

void insert_on_node(Pointer** tree, Register* reg, Pointer** right){
    int k = (*tree)->n;
    bool not_find_position = (k > 0);
    //Caso a página tenha itens, rearanja para inserir na posição correta
    while( not_find_position){
        if( reg->key >= (*tree)->r[k-1].key ){
            not_find_position = false;
            break;
        }
        (*tree)->r[k] = (*tree)->r[k-1];
        (*tree)->p[k+1] = (*tree)->p[k];
        k--;
        if( k < 1){
            not_find_position = false;
        }
    }

    (*tree)->r[k] = *reg;
    (*tree)->p[k+1] = *right;
    (*tree)->n++;
}

