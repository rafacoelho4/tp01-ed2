#include "b_star.hpp"

void b_star(int key, const char* file_name, int qtd, bool p) {
	Register reg;
	long int transfer = 0, compare = 0;
	Page* tree;
	tree = NULL;
    FILE* input_file;

    // abre o arquivo de dados
    if ((input_file = fopen(file_name, "rb")) == NULL) {
		cout<<"Erro na abertura do arquivo\n";
		exit(1);
	}

   transfer++;
   clock_t start = clock();
   while (fread(&reg,sizeof(Register),1,input_file) == 1)
   {
        insert_general(&reg,&tree,&transfer,&compare);
        transfer++;
   }
    clock_t end = clock();
    cout<<"Tempo inserção:\n";
    cout<<"Quantidade de comparações: "<<(compare)<<endl;
    cout<<"Quantidade de transferencias: "<<transfer<<endl;
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout<<"Tempo total: "<<cpu_time_used<<endl;

   reg.key = key; compare = 0; transfer = 0;
   start = clock();
   if (p){
        print(&tree, key, &transfer, &compare);
   }

   search(&reg, &tree, &transfer, &compare);
    end = clock();
      cout<<"Tempo pesquisa:\n";
    cout<<"Quantidade de comparações: "<<(compare)<<endl;
    cout<<"Quantidade de transferencias: "<<transfer<<endl;
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    cout<<"Tempo total: "<<cpu_time_used<<endl;

   fclose(input_file);
}

// funcao que pesquisa pela arvore b estrela
void search(Register* it, PointerS *p, long int* transfer, long int* compare) {
    int i;

    PointerS pag = *p;

	// decide qual destino a seguir
    if ((*p)->st == INTERNAL){
        i = 1;

		// pesquisa o intervalo em que o registro pode estar
		*compare += 2;
        while (i < pag->UU.UI.n_int && it->key > pag->UU.UI.key_int[i - 1]){
			*compare += 2;
			i++;
		}

		// seleciona o lado (direita ou esquerda) que deve ser pesquisado ate achar o registro
        if (it->key < pag->UU.UI.key_int[i - 1]){
			search(it, &pag->UU.UI.p_int[i - 1], transfer, compare);
		}

        else{
			search(it, &pag->UU.UI.p_int[i], transfer, compare);
		}
        return;
    }

    i = 1;

	*compare += 1;
    while (i < pag->UU.UE.n_ext && it->key > pag->UU.UE.reg_ext[i - 1].key){
		*compare += 1;
		i++;
	}

	*compare += 1;
    if(it->key == pag->UU.UE.reg_ext[i - 1].key){
		*it = pag->UU.UE.reg_ext[i - 1];
		printf("Registro encontrado\n");
	}

    else{
		printf("Registro não encontrado!\n");
	}
}



// inicia a insercao da b estrela
void insert_general(Register* reg, PointerS* pointer, long int* transfer, long int* compare){
	short grown;

	Register regReturn, temp = *reg;
	Page *ApReturn, *ApTemp;
	/*
		inicializa a arvore b estrela caso ela seja vazia e inicializa a arvore b caso seja necessario
	*/
	if (*pointer == NULL){
		ApTemp = (Page*) calloc(1, sizeof(Page));
		ApTemp->st = EXTERNAL;
		ApTemp->UU.UE.n_ext = 1;
		ApTemp->UU.UE.reg_ext[0] = temp;
		*pointer = ApTemp;
		return;
	}
	else{
		ins_Bstar(temp, *pointer, &grown, &regReturn, &ApReturn, transfer, compare);
		if (grown == 1){
			ApTemp = (Page*) calloc(1, sizeof(Page));
			ApTemp->st = INTERNAL;
			ApTemp->UU.UI.n_int = 1;
			ApTemp->UU.UI.key_int[0] = regReturn.key;
			ApTemp->UU.UI.p_int[1] = ApReturn;
			ApTemp->UU.UI.p_int[0] = *pointer;
			*pointer = ApTemp;
		}
	}
}

// funcao que insere uma nova chave na parte externa da arvore
void ins_Bstar(Register reg, PointerS pointer, short* grown, Register* regReturn, PointerS*ApReturn, long int* transfer, long int* compare){
	int i = 1;

	if (pointer->st == EXTERNAL) {
		*compare += 1;
		if (pointer->UU.UE.n_ext < MM){
			*regReturn = reg;
			insert_on_page(pointer, *regReturn, *ApReturn, transfer, compare);
			*grown = 0;
			return;
		}
		else{
			*regReturn = reg;
			*ApReturn = NULL;

			*compare += 1;
			while (i < pointer->UU.UE.n_ext && reg.key > pointer->UU.UE.reg_ext[i-1].key){
				i++;
				*compare += 1;
			}

			*compare += 1;

			if(reg.key < pointer->UU.UE.reg_ext[i-1].key) i--;

			PointerS temp = (PointerS) malloc(sizeof(Page));

			temp->st = EXTERNAL;
			temp->UU.UE.n_ext = 0;

			if (i < (M+1)){
				insert_on_page(temp, pointer->UU.UE.reg_ext[MM-1], *ApReturn, transfer, compare);
				pointer->UU.UE.n_ext--;
				insert_on_page(pointer, *regReturn, *ApReturn, transfer, compare);
			}
			else{
				insert_on_page(temp, *regReturn, *ApReturn, transfer, compare);
			}

			for (int j = 1; j <= M; j++){
				insert_on_page(temp, pointer->UU.UE.reg_ext[MM - j], *ApReturn, transfer, compare);
			}

			pointer->UU.UE.n_ext = M;
			*regReturn = pointer->UU.UE.reg_ext[M];
			*ApReturn = temp;
			*grown = 1;
			return;
		}
	}
	insert_Bstar(pointer, ApReturn, regReturn, reg, grown, i, transfer, compare);
}

// funcao que insere seleciona a ramificacao adequada para insercao
void insert_Bstar(PointerS pointer, PointerS* ApReturn, Register* regReturn, Register reg, short* grown, long i, long int* transfer, long int* compare){
	*compare += 1;

	while (i < pointer->UU.UI.n_int && reg.key > pointer->UU.UI.key_int[i-1]){
		*compare += 1;
		i++;
	}

	*compare += 1;
	if(reg.key < pointer->UU.UI.key_int[i-1]) i--;

	ins_Bstar(reg, pointer->UU.UI.p_int[i], grown, regReturn, ApReturn, transfer, compare);
	if(*grown == 0) return;

	if (pointer->UU.UI.n_int < MM){
		insert_on_page(pointer, *regReturn, *ApReturn, transfer, compare);
		*grown = 0;
		return;
	}

	*compare += 1;
	if (reg.key == pointer->UU.UI.key_int[i-1]) {
		*grown = 0;
		return;
	}

	PointerS temp = (PointerS)calloc(1,sizeof(Page));

	temp->st = INTERNAL;
	temp->UU.UI.n_int = 0;
	temp->UU.UI.p_int[0] = NULL;

	Register regTemp;

	if (i < (M + 1)){
		regTemp.key = pointer->UU.UI.key_int[MM - 1];
		insert_on_page(temp, regTemp, pointer->UU.UI.p_int[MM], transfer, compare);
		pointer->UU.UI.n_int--;
		insert_on_page(pointer, *regReturn, *ApReturn, transfer, compare);
	}
	else{
		insert_on_page(temp, *regReturn, *ApReturn, transfer, compare);
	}

	for (int j = M + 2; j <= MM; j++){
		regTemp.key = pointer->UU.UI.key_int[j-1];
		insert_on_page(temp, regTemp, pointer->UU.UI.p_int[j], transfer, compare);
	}

	pointer->UU.UI.n_int = M;
	temp->UU.UI.p_int[0] = pointer->UU.UI.p_int[M+1];
	regReturn->key = pointer->UU.UI.key_int[M];
	*ApReturn = temp;
	return;
}

// funcao que insere na pagina o registro
void insert_on_page(PointerS pointer, Register reg, PointerS right, long int* transfer, long int* compare){
	short position;
	int k;

	if (pointer->st == EXTERNAL){
		k = pointer->UU.UE.n_ext;
		position = (k > 0);

		*compare += 1;
		while (position > 0){
			*compare += 1;
			if (reg.key >= pointer->UU.UE.reg_ext[k-1].key){
				position = 0;
				break;
			}
			pointer->UU.UE.reg_ext[k] = pointer->UU.UE.reg_ext[k-1];
			k--;

			if(k < 1) position = 0;
		}

		pointer->UU.UE.reg_ext[k] = reg;
		pointer->UU.UE.n_ext++;
	}
	else{
		k = pointer->UU.UI.n_int;
		position = (k > 0);

		while (position > 0){
			*compare += 1;
			if (reg.key >= pointer->UU.UI.key_int[k-1]){
				position = 0;
				break;
			}
			pointer->UU.UI.key_int[k] = pointer->UU.UI.key_int[k-1];
			pointer->UU.UI.p_int[k+1] = pointer->UU.UI.p_int[k];
			k--;
			if(k < 1) position = 0;
		}
		pointer->UU.UI.key_int[k] = reg.key;
		pointer->UU.UI.p_int[k+1] = right;
		pointer->UU.UI.n_int++;
		return;
	}
}

// funcao que imprime o caminho percorrido ate encontrar a chave desejada
void print(PointerS* pointer, int key, long int* transfer, long int* compare){
	int i = 0;
	PointerS aux;

	if (pointer == NULL){
		return;
	}

	if ((*pointer)->st == INTERNAL) {
		while (i <= (*pointer)->UU.UI.n_int){
			print(&((*pointer)->UU.UI.p_int[i]), key, transfer, compare);
			if (i != (*pointer)->UU.UI.n_int)
				printf("Interno: (%d) %d\n", i, (*pointer)->UU.UI.key_int[i]);
			i++;
		}
	}
	else{
		printf("Externo: ");
		aux = (*pointer);
		for (int j = 0; j < (*pointer)->UU.UE.n_ext; j++) {
			printf("(%d) %d ", j, aux->UU.UE.reg_ext[j].key);
		}
		printf("\n");
	}
}



