#include "b_tree.hpp"
using namespace std;

#define M 2

void b_tree(int key, const char* name, bool parametro){
    //Arquivo de entrada
    FILE* input_file;
    //Alocando o ponteiro para a raiz da arvore
    Pointer* tree =(Pointer*) malloc( sizeof(NodeT));
    //Setando a raiz da arvore em Nulo no primeiro momento
    tree = NULL;
    //Criando a variavel auxilar de registro
    Register r;
    //Criando contadores de transferencias e comparaçoes
    long long transf = 0, comp = 0;
    //Verificando se o arquivo foi aberto com sucesso
    if(open_binary_file( &input_file,name,"rb")){
        //Lendo os registros do arquivo e inserindo na arvore
        clock_t start = clock();
        (transf)++;
        while (fread(&r,sizeof(Register),1,input_file) == 1)
        {
            insert( r, &tree, &comp );
            (transf)++;
        }
        clock_t end = clock();
        cout<<"Tempo inserção:\n";
        cout<<"Quantidade de comparações: "<<(comp)<<endl;
        cout<<"Quantidade de transferencias: "<<transf<<endl;
        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout<<"Tempo total: "<<cpu_time_used<<endl;   
       
        r.key = key;
        comp = 0;
        start = clock();
        if(search(&r,tree,&comp,parametro )){
            cout<<"\nRegistro encontrado!";
            printf("Item: %d | %ld, %s \n", r.key, r.data1, r.data2);
        }
        else{
             cout<<"\nRegistro não foi encontrado!";
        } 
        end = clock();
        cout<<"\nTempo busca: \n";
        cout<<"Quantidade de comparações: "<<(comp)<<endl;
        cout<<"Quantidade de transferencias: "<<transf<<endl;
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout<<"Tempo total: "<<cpu_time_used<<endl;
       
    } 
    fclose(input_file);          
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

bool search( Register* r, Pointer* tree,long long *comp, bool parametro ){
     int i = 1;

    if( tree == NULL){
        return false;
    }

    //Caminha na página até encontrar o local desejado
    (*comp)++;
     while(i < tree->n && r->key > tree->r[i-1].key){
        cout<<"\noi";
        if(parametro)
            cout<<"\n"<< tree->r[i-1].key<<" "<<tree->n;
        i++;
        (*comp)++;
     } 

    //Compara o valor, caso seja igual : retorna verdadeiro e a pesquisa se encerra
    (*comp)++;
     if(r->key == tree->r[i-1].key){
         if(parametro)
            cout<<"\n"<< tree->r[i-1].key;
        *r = tree->r[i-1];
        return true;
    }
    
    //Caso o valor do registro seja menor que o ultimo registro da pagina: caminhamento à esquerda
    (*comp)++;
    if(r->key < tree->r[i-1].key){
        search( r, tree->p[i-1], comp,parametro);
    }
    
    //Caso contrário: caminhamento à direita
    
    else search( r, tree->p[i],comp,parametro);  
    
} 


void insert(Register reg, Pointer** tree,long long *comp){
    
    //Variável responsavel por verificar se a arvore cresceu
    bool grew;
    //Armazena o registro de retorno
    Register rr;
    //Armazena filho à direita do registro retornado
    Pointer *pr;
    //Ponteiro de páginas temporários
    Pointer* p_temp;
    //Realiza uma busca na arvore para inserir o registro reg
    find_to_insert(reg, tree, &grew, &rr, &pr, comp);
     
    
    //Criação de um novo nó raiz
     if(grew){
        p_temp  =(Pointer*) malloc( sizeof(NodeT));
        p_temp->n = 1;
        p_temp->r[0] = rr;
        p_temp->p[1] = pr;
        p_temp->p[0] = (*tree);
        *tree = p_temp;
    }
    comp++; 
}

void find_to_insert(Register reg, Pointer** tree, bool *grew, Register* rr, Pointer** pr,long long *comp){
    //Variável que caminha pelas paginas
    long i = 1;
    //Variável responsável por definir local de divisão dos filhos à direita da página
    long j;
    //Ponteiro de páginas temporário
    Pointer* p_temp;
    
    //Verifica se arvore está vazia e retorna os valores para a criação da raiz
   (*comp)++;
   if(*tree == NULL){
        *grew = true;
        *rr = reg;
        *pr = NULL;
        return; 
    }  

     //Laço que caminha pela pagina apontada pela raiz da arvore
    (*comp)++;
    while(i < (*tree)->n && reg.key >(*tree)->r[i-1].key){  i++;(*comp)++;};
    
    //Verificação caso o registro já esteja presente na arvore
    
    (*comp)++;
    if(reg.key ==(*tree)->r[i-1].key){
        cout<<"Erro: O registro já se encontra na arvore";
        *grew = false;
        return;
    }
    
    //A proxima procura será com o filho a esquerda caso a chave seja menor que o ultimo registro
    (*comp)++;
    if(reg.key < (*tree)->r[i-1].key) i--;

    //Realiza a pesquisa na subarvore selecionada
    find_to_insert(reg, &((*tree)->p[i]), grew, rr, pr,comp);

    //Verifica se será necessaria a criação de uma nova pagina
    (*comp)++;
    if(!(*grew)) return;

    //Caso a pagina tenha espaço
    (*comp)++;
    if( (*tree)->n < (M*M)){
        insert_on_node( tree, rr, pr,comp);
        *grew = false;
        return;
    }

    //Caso a pagina precise ser dividida
    //Aloca ponteiro auxiliar e inicializa
    p_temp = (Pointer*) malloc( sizeof(NodeT) );
    p_temp->n = 0;
    p_temp->p[0] = NULL;

    (*comp)++;
    if(i < (M+1)){
        insert_on_node(&p_temp,&(*tree)->r[(M*M)-1],&(*tree)->p[(M*M)],comp);
        (*tree)->n--;
        insert_on_node(tree, rr, pr,comp);
    }

    else {(*comp)++; insert_on_node(&p_temp, rr, pr,comp);}

    //Processo de divisão das paginas
    for( j = M + 2; j <=(M*M); j++ ){
        insert_on_node(&p_temp,&(*tree)->r[j-1],&(*tree)->p[j],comp);
    }
    
   (*tree)->n = M;
    p_temp->p[0] = (*tree)->p[M+1];
    *rr = (*tree)->r[M];
    *pr = p_temp;
}

void insert_on_node(Pointer** tree, Register* reg, Pointer** right, long long* comp){
    int k = (*tree)->n;
    bool not_find_position = (k > 0);
    //Caso a página tenha itens, rearanja para inserir na posição correta
    while( not_find_position){
       (*comp)++;
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

