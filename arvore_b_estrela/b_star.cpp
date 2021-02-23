#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define M 2
#define MM 2*M
#define SIZE 200

// estrutura criada a partir do arquivo binario
typedef struct{
	int key;
	long data1;
	char data2[1000];
}Register;

typedef enum {
    INTERNAL,
    EXTERNAL
} Status;

typedef struct Node* Pointer;

typedef struct Node {
    Status st;

    union {
        struct {
            int n_int;
            int key_int[MM];
            Pointer p_int[MM + 1];
        } UI;

        struct {
            int n_ext;
            Register reg_ext[MM];
        } UE;
    } UU;
} Node;

void insert(Register* reg, Pointer* pointer){
	short grew;

	Register rr, temp = *reg;
	Node *pr, *p_temp;
	/*
		inicializa a arvore b estrela caso ela seja vazia e inicializa a arvore b caso seja necessario
	*/
	if (*pointer == NULL){
		p_temp = (Node*) calloc(1, sizeof(Node));
		p_temp->st = EXTERNAL;
		p_temp->UU.UE.n_ext = 1;
		p_temp->UU.UE.reg_ext[0] = temp;
		*pointer = p_temp;
		return;
	}
	else{
		insBStar(temp, *pointer, &grew, &rr, &pr);
		if (grew == 1){
			p_temp = (Node*) calloc(1, sizeof(Node));
			p_temp->st = INTERNAL;
			p_temp->UU.UI.n_int = 1;
			p_temp->UU.UI.key_int[0] = rr.key;
			p_temp->UU.UI.p_int[1] = pr;
			p_temp->UU.UI.p_int[0] = *pointer;
			*pointer = p_temp;
		}
	}
}

// funcao que insere uma nova chave na parte externa da arvore
void insBStar(Register reg, Pointer pointer, short* grew, Register* rr, Pointer*pr ){
	int i = 1;

	if (pointer->st == EXTERNAL) {
		
		if (pointer->UU.UE.n_ext < MM){
			*rr = reg;
			insertOnNodeStar(pointer, *rr, *pr);
			*grew = 0;
			return;
		}
		else{
			*rr = reg;
			*pr = NULL;

			
			while (i < pointer->UU.UE.n_ext && reg.key > pointer->UU.UE.reg_ext[i-1].key){
				i++;
				
			}
			if(reg.key < pointer->UU.UE.reg_ext[i-1].key) i--;

			Pointer temp = (Pointer) calloc(1,sizeof(Node));

			temp->st = EXTERNAL;
			temp->UU.UE.n_ext = 0;

			if (i < (M+1)){
				insertOnNodeStar(temp, pointer->UU.UE.reg_ext[MM-1], *pr);
				pointer->UU.UE.n_ext--;
				insertOnNodeStar(pointer, *rr, *pr);
			}
			else{
				insertOnNodeStar(temp, *rr, *pr);
			}

			for (int j = 1; j <= M; j++){
				insertOnNodeStar(temp, pointer->UU.UE.reg_ext[MM - j], *pr);
			}

			pointer->UU.UE.n_ext = M;
			*rr = pointer->UU.UE.reg_ext[M];
			*pr = temp;
			*grew = 1;
			return;
		}
	}
	insertBStar(pointer, pr, rr, reg, grew, i);
}

// funcao que insere seleciona a ramificacao adequada para insercao
void insertBStar(Pointer pointer, Pointer* pr, Register* rr, Register reg, short* grew, long i ){
	

	while (i < pointer->UU.UI.n_int && reg.key > pointer->UU.UI.key_int[i-1]){
		
		i++;
	}

	
	if(reg.key < pointer->UU.UI.key_int[i-1]) i--;

	insBStar(reg, pointer->UU.UI.p_int[i], grew, rr, pr );
	if(*grew == 0) return;

	if (pointer->UU.UI.n_int < MM){
		insertOnNodeStar(pointer, *rr, *pr );
		*grew = 0;
		return;
	}

	
	if (reg.key == pointer->UU.UI.key_int[i-1]) {
		*grew = 0;
		return;
	}

	Pointer temp = (Pointer)calloc(1,sizeof(Node));

	temp->st = INTERNAL;
	temp->UU.UI.n_int = 0;
	temp->UU.UI.p_int[0] = NULL;

	Register regTemp;

	if (i < (M + 1)){
		regTemp.key = pointer->UU.UI.key_int[MM - 1];
		insertOnNodeStar(temp, regTemp, pointer->UU.UI.p_int[MM]);
		pointer->UU.UI.n_int--;
		insertOnNodeStar(pointer, *rr, *pr);
	}
	else{
		insertOnNodeStar(temp, *rr, *pr);
	}

	for (int j = M + 2; j <= MM; j++){
		regTemp.key = pointer->UU.UI.key_int[j-1];
		insertOnNodeStar(temp, regTemp, pointer->UU.UI.p_int[j]);
	}

	pointer->UU.UI.n_int = M;
	temp->UU.UI.p_int[0] = pointer->UU.UI.p_int[M+1];
	rr->key = pointer->UU.UI.key_int[M];
	*pr = temp;
	return;
}

// funcao que insere na pagina o registro
void insertOnNodeStar(Pointer pointer, Register reg, Pointer right ){
	short position;
	int k;

	if (pointer->st == EXTERNAL){
		k = pointer->UU.UE.n_ext;
		position = (k > 0);

		
		while (position > 0){
		
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