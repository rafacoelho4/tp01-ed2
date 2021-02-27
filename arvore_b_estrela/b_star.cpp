#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include "../tipos.hpp"

using namespace std;

#define M 2
#define MM 2*M
#define SIZE 200

typedef enum {
    INTERNA,
    EXTERNA
} Status;

typedef struct Nodo* Apontador;

typedef struct Nodo {
    Status st;

    union {
        struct {
            int n_int;
            int chave_int[MM];
            Apontador p_int[MM + 1];
        } UI;

        struct {
            int n_ext;
            Item item_ext[MM];
        } UE;
    } UU;
} Nodo;

int abre_arquivo_bin(FILE **file, const char* name, const char* type);
void b_star(int chave, const char* arq, int size, bool P);
void imprimir(Apontador* ap, int chave, long int* transfer, long int* compare);
Item** ler_arquivo(FILE** file, int size, long int* transfer, long int* compare);
void inserir(Item* item, Apontador* ap, long int* transfer, long int* compare);
void inserir_bstar(Apontador ap, Apontador* pr, Item* rr, Item item, short* cresceu, long i, long int* transfer, long int* compare);
void ins_bstar(Item item, Apontador ap, short* cresceu, Item* rr, Apontador*pr, long int* transfer, long int* compare);
void inserir_no_nodo_star(Apontador ap, Item item, Apontador direita, long int* transfer, long int* compare);
void pesquisa_bstar(Item* item, Apontador *p, long int* transfer, long int* compare);

int abre_arquivo_bin(FILE **file, const char* name, const char* type){
    if((*file = fopen(name,type)) == NULL){
        return 0;
    }
    return 1;
}

void b_star(int chave, const char* arq, int size, bool P){
	clock_t g1 = clock();
	long int comp = 0, transf = 0;
    FILE* input_file;
	Apontador* arvore = NULL;

    if(abre_arquivo_bin(&input_file, arq, "r+b")){
        //cout << "\n\n\n\noi" << arq;

		// Cria um vetor com os itens do arquivo binário
		Item** itens = ler_arquivo(&input_file, size, &transf, &comp);
		Item* item = (Item*) calloc(1, sizeof(Item));
		item->chave = chave;

		// Insere os itens do arquivo binário na árvore
		for (int i = 0; i < size; i++) {
            cout<<"\nINSERINDO ITEM %d\n" << i;
			inserir(itens[i], arvore, &transf, &comp);
		}

	/* 	comp = 0; transf = 0;

		g1 = (clock() - g1)/(CLOCKS_PER_SEC/1000);
		printf("Insercao Arvore B*\n\t- Tempo: %15lld ms\n\t- Comparacoes: %12ldx\n\t- Tranferencias: %9ldx\n\n",
		(long long unsigned int)g1, comp, transf);

		clock_t g2 = clock();
		comp = 0; // ERROR: *comp = 0 ?? n da certo assim

		pesquisa_bstar(item, arvore, &transf, &comp);

		g2 = (clock() - g2)/(CLOCKS_PER_SEC/1000);
		printf("Pesquisa Arvore B\n\t- Tempo: %15lld ms\n\t- Comparacoes: %12ldx\n\t- Tranferencias: %9ldx\n\n",
		(long long unsigned int)g2, comp, transf);

		if(P){ // imprime a árvore
		//printBStarTree(tree, key, transference, compare);
		imprimir(arvore, chave, transf, comp);
		}

		for (size_t i = 0; i < size; i++){
        	free(itens[i]);
    	}

    	free(item); */
    } 
}

void imprimir(Apontador* ap, int chave, long int* transfer, long int* compare){
	if(ap == NULL){
		return;
	}

	int i = 0;
	Apontador aux;

	if ((*ap)->st == INTERNA) {
		while (i <= (*ap)->UU.UI.n_int){
			imprimir(&((*ap)->UU.UI.p_int[i]), chave, transfer, compare);
			if (i != (*ap)->UU.UI.n_int)
				printf("Interno: (%d) %d\n", i, (*ap)->UU.UI.chave_int[i]);
			i++;
		}
	}
	else{
		printf("Externo: ");
		aux = (*ap);
		for (int j = 0; j < (*ap)->UU.UE.n_ext; j++) {
			printf("(%d) %d ", j, aux->UU.UE.item_ext[j].chave);
		}
		printf("\n");
	}

}

Item** ler_arquivo(FILE** file, int size, long int* transfer, long int* compare){
	Item aux;
	Item** item = (Item**) calloc(size, sizeof(Item));
	for (int i = 0; i < size; i++) {
		item[i] = (Item*) calloc(1,sizeof(Item));
	}

	// Lê registro por registro e insere na variável auxiliar criada
	int i = 0;
	while (fread(&aux, sizeof(Item), 1, *file) && size > i){
		*transfer += 1;
		Item* aux1 = (Item*) calloc(1, sizeof(Item));
		aux1->chave = aux.chave;
		aux1->dado1 = aux.dado1;
		strcpy(aux1->dado2, aux.dado2);
		item[i] = aux1;
		i++;
	}

	return item;
}

// Transfer e compare são contadores p/ o nº de transferências e comparações
void inserir(Item* item, Apontador* ap, long int* transfer, long int* compare){
	short cresceu;

	Item rr, temp = *item;
	Nodo *pr, *p_temp;

	if (*ap == NULL){ // Inicializa a Árvore B* caso necessário
		p_temp = (Nodo*) calloc(1, sizeof(Nodo));
		p_temp->st = EXTERNA;
		p_temp->UU.UE.n_ext = 1;
		p_temp->UU.UE.item_ext[0] = temp;
		*ap = p_temp;
		return;
	}
	else{
		ins_bstar(temp, *ap, &cresceu, &rr, &pr, transfer, compare);
		if (cresceu == 1){
			p_temp = (Nodo*) calloc(1, sizeof(Nodo));
			p_temp->st = INTERNA;
			p_temp->UU.UI.n_int = 1;
			p_temp->UU.UI.chave_int[0] = rr.chave;
			p_temp->UU.UI.p_int[1] = pr;
			p_temp->UU.UI.p_int[0] = *ap;
			*ap = p_temp;
		}
	}
}

// Função que insere uma nova chave na página externa da árvore
void ins_bstar(Item item, Apontador ap, short* cresceu, Item* rr, Apontador*pr, long int* transfer, long int* compare){
	int i = 1;

	if (ap->st == EXTERNA) {
		*compare += 1;
		if (ap->UU.UE.n_ext < MM){
			*rr = item;
			inserir_no_nodo_star(ap, *rr, *pr, transfer, compare);
			*cresceu = 0;
			return;
		}
		else{
			*rr = item;
			*pr = NULL;
		
			*compare += 1;
			while (i < ap->UU.UE.n_ext && item.chave > ap->UU.UE.item_ext[i-1].chave){
				i++;
				*compare += 1;	
			}

			*compare += 1;

			if(item.chave < ap->UU.UE.item_ext[i-1].chave) i--;

			Apontador temp = (Apontador) calloc(1,sizeof(Nodo));

			temp->st = EXTERNA;
			temp->UU.UE.n_ext = 0;

			if (i < (M+1)){
				inserir_no_nodo_star(temp, ap->UU.UE.item_ext[MM-1], *pr, transfer, compare);
				ap->UU.UE.n_ext--;
				inserir_no_nodo_star(ap, *rr, *pr, transfer, compare);
			}
			else{
				inserir_no_nodo_star(temp, *rr, *pr, transfer, compare);
			}

			for (int j = 1; j <= M; j++){
				inserir_no_nodo_star(temp, ap->UU.UE.item_ext[MM - j], *pr, transfer, compare);
			}

			ap->UU.UE.n_ext = M;
			*rr = ap->UU.UE.item_ext[M];
			*pr = temp;
			*cresceu = 1;
			return;
		}
	}
	inserir_bstar(ap, pr, rr, item, cresceu, i, transfer, compare);
}

// Função que insere seleciona a ramificacão adequada p/ inserção
void inserir_bstar(Apontador ap, Apontador* pr, Item* rr, Item item, short* cresceu, long i, long int* transfer, long int* compare){
	*compare += 1;
	while (i < ap->UU.UI.n_int && item.chave > ap->UU.UI.chave_int[i-1]){
		*compare += 1;
		i++;
	}

	*compare += 1;
	if(item.chave < ap->UU.UI.chave_int[i-1]) i--;

	ins_bstar(item, ap->UU.UI.p_int[i], cresceu, rr, pr, transfer, compare);
	if(*cresceu == 0) return;

	if (ap->UU.UI.n_int < MM){
		inserir_no_nodo_star(ap, *rr, *pr, transfer, compare);
		*cresceu = 0;
		return;
	}
	
	if (item.chave == ap->UU.UI.chave_int[i-1]) {
		*cresceu = 0;
		return;
	}

	Apontador temp = (Apontador)calloc(1,sizeof(Nodo));

	temp->st = INTERNA;
	temp->UU.UI.n_int = 0;
	temp->UU.UI.p_int[0] = NULL;

	Item itemTemp;

	if (i < (M + 1)){
		itemTemp.chave = ap->UU.UI.chave_int[MM - 1];
		inserir_no_nodo_star(temp, itemTemp, ap->UU.UI.p_int[MM], transfer, compare);
		ap->UU.UI.n_int--;
		inserir_no_nodo_star(ap, *rr, *pr, transfer, compare);
	}
	else{
		inserir_no_nodo_star(temp, *rr, *pr, transfer, compare);
	}

	for (int j = M + 2; j <= MM; j++){
		itemTemp.chave = ap->UU.UI.chave_int[j-1];
		inserir_no_nodo_star(temp, itemTemp, ap->UU.UI.p_int[j], transfer, compare);
	}

	ap->UU.UI.n_int = M;
	temp->UU.UI.p_int[0] = ap->UU.UI.p_int[M+1];
	rr->chave = ap->UU.UI.chave_int[M];
	*pr = temp;
	return;
}

// Função que insere na página o item
void inserir_no_nodo_star(Apontador ap, Item item, Apontador direita, long int* transfer, long int* compare){
	short posicao;
	int k;

	if (ap->st == EXTERNA){
		k = ap->UU.UE.n_ext;
		posicao = (k > 0);

		*compare += 1;
		while (posicao > 0){
			*compare += 1;
			if (item.chave >= ap->UU.UE.item_ext[k-1].chave){
				posicao = 0;
				break;
			}
			ap->UU.UE.item_ext[k] = ap->UU.UE.item_ext[k-1];
			k--;

			if(k < 1) posicao = 0;
		}

		ap->UU.UE.item_ext[k] = item;
		ap->UU.UE.n_ext++;
	}
	else{
		k = ap->UU.UI.n_int;
		posicao = (k > 0);

		while (posicao > 0){
			*compare += 1;
			if (item.chave >= ap->UU.UI.chave_int[k-1]){
				posicao = 0;
				break;
			}
			ap->UU.UI.chave_int[k] = ap->UU.UI.chave_int[k-1];
			ap->UU.UI.p_int[k+1] = ap->UU.UI.p_int[k];
			k--;
			if(k < 1) posicao = 0;
		}
		ap->UU.UI.chave_int[k] = item.chave;
		ap->UU.UI.p_int[k+1] = direita;
		ap->UU.UI.n_int++;
		return;
	}
}

// Função que faz a pesquisa dentro da árvore
void pesquisa_bstar(Item* item, Apontador *p, long int* transfer, long int* compare){
    int i;

    Apontador pag = *p;

	// Pesquisa na página interna
    if ((*p)->st == INTERNA){
        i = 1;

		// Pesquisa o intervalo no qual o registro pode estar
		*compare += 2;
        while (i < pag->UU.UI.n_int && item->chave > pag->UU.UI.chave_int[i - 1]){
			*compare += 2;
			i++;
		}

		// Seleciona o lado da árvore (direita ou esquerda) onde se deve pesquisar até achar uma pág. externa
        if (item->chave < pag->UU.UI.chave_int[i - 1]){
			pesquisa_bstar(item, &pag->UU.UI.p_int[i - 1], transfer, compare);
		}
        else{
			pesquisa_bstar(item, &pag->UU.UI.p_int[i], transfer, compare);
		}

        return;
    }

    i = 1;

	*compare += 1;
    while (i < pag->UU.UE.n_ext && item->chave > pag->UU.UE.item_ext[i - 1].chave){
		*compare += 1;
		i++;
	}

	*compare += 1;

    if (item->chave == pag->UU.UE.item_ext[i - 1].chave){
		*item = pag->UU.UE.item_ext[i - 1];
		printf("Registro encontrado!\n");
	}
    else{
		printf("Registro não encontrado!\n");
	}
}