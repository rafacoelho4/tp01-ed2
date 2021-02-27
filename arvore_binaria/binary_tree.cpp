#include "binary_tree.hpp"

void binary_tree( int key, const char* name, bool parametro ){
    
    long long comp=0, transf=0;
    FILE* output_file;
    FILE* input_file;
    //caso o arquivo abra com sucesso, executa:
     if(open_binary_file( &output_file,"arvore.bin","w+b")&& open_binary_file( &input_file,name,"r+b")){
      
        cout<<"\n\n\n\noi"<<name;
        insert(output_file, input_file, &transf, &comp);
        comp=0; transf=0;
        //Volta para o início do arquivo
        rewind(output_file);
        //realiza a pesquisa do item 18
        search( key, output_file, parametro);      
    } 
    fclose(output_file);
    fclose(input_file);             
}

void search(int key, FILE* output_file, bool parametro){
     long long transf=0;  
     long long comp=0;
    clock_t start = clock();
        if(search_node( key, &output_file, &transf, &comp,parametro)){
            cout<<"item encontrado"<<endl;
        }
        else{
            cout<<"item não encontrado"<<endl;
        }
         clock_t end = clock();
        cout<<"Tempo busca:\n";
        cout<<"Quantidade de comparações: "<<comp<<endl;
        cout<<"Quantidade de transferencias: "<<(transf)<<endl;
        double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        cout<<"Tempo total: "<<cpu_time_used<<endl;     
}

void insert(FILE* output_file, FILE* input_file,  long long*transf ,  long long*comp){
    clock_t start = clock();
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
                        inserted = insert_right_node( a, aux, i, &position, &output_file);
                        (*comp)++;
                    }
                    //Caso a chave seja menor do que do item lido, é feita a tentativa de inserir à esquerda
                    else if(a.reg.key < aux.reg.key){
                        (*comp)+=2;
                        inserted = insert_left_node( a, aux, i, &position, &output_file );
                        (*comp)++;
                    }
                        //Caso o item seja igual, é feita a finalização do laço
                    else{
                        (*comp)+=3;
                        inserted = 1;
                    }
                    
                    
                } 
            } 
        }
        (*transf)++;    
    } 
    clock_t end = clock();
    cout<<"Tempo inserção:\n";
    cout<<"Quantidade de comparações: "<<(*comp)<<endl;
    cout<<"Quantidade de transferencias: "<<(*transf)<<endl;
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout<<"Tempo total: "<<cpu_time_used<<endl;        
}

Node create_node(Node a ){
    a.right = -1;
    a.left = -1;
    return a;
}

int open_binary_file(FILE **file, const char* name, const char* type){
    if((*file = fopen(name,type)) == NULL){
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

 int search_node( int key, FILE** file,  long long*transf , long long*comp, bool parametro){
    
    Node aux;
    int position = 0;
    (*transf)++;
    while ( fread(&aux, sizeof( Node), 1, *file) == 1 )
    {
        fseek( *file, position*sizeof(Node), SEEK_SET);
    
        if(fread( &aux, sizeof(Node), 1, *file ) == 1){
            (*transf)++;
            if(parametro)
                cout<<aux.reg.key;
                
            if(key > aux.reg.key){
                (*comp)++;
                position = aux.left;
            }
            else if(key < aux.reg.key){
                (*comp)+=2;
                position = aux.right;
            }
            else{
                (*comp)+=3;
                return 1;
            }
        } 
        (*transf)++;
    }
      

    return 0;
} 