#include <stdio.h>
#include <stdlib.h>

#define RED   1
#define BLACK 0

typedef struct arvoreRB {
  int info;
  int cor;
  struct arvoreRB *esq;
  struct arvoreRB *dir;
} ArvoreRB;

int eh_no_vermelho(ArvoreRB * no){
  if(!no) return BLACK;
  return(no->cor == RED);
}

int buscar (ArvoreRB *a, int v) {
  if (a == NULL) { return 0; } /*Nao achou*/
  else if (v < a->info) {
    return buscar (a->esq, v);
  }
  else if (v > a->info) {
    return buscar (a->dir, v);
  }
  else { return 1; } /*Achou*/
}

void in_order(ArvoreRB *a){
  if(!a)
    return;
  in_order(a->esq);
  printf("%d ",a->info);
  in_order(a->dir);
}

void print(ArvoreRB * a,int spaces){
  int i;
  for(i=0;i<spaces;i++) printf(" ");
  if(!a){
    printf("//\n");
    return;
  }

  printf("%d\n", a->info);
  print(a->esq,spaces+2);
  print(a->dir,spaces+2);
}

ArvoreRB * rot_esq(ArvoreRB * no) {
	ArvoreRB * x = no->dir;
	no->dir = x->esq;
	x->esq = no;
	x->cor = no->cor;
	no->cor = RED;
	return x;
}

ArvoreRB * rot_dir(ArvoreRB * no) {
	ArvoreRB * x = no->esq;
	no->esq = x->dir;
	x->dir = no;
	x->cor = no->cor;
	no->cor = RED;
	return x;
}

ArvoreRB * flip_cor(ArvoreRB * no) {
	no-> cor = RED;
	no->esq->cor = BLACK;
	no->dir->cor = BLACK;
	return no;
}

void inserir(ArvoreRB * no, ArvoreRB * novo){
	//tengho que descer ate o ultimo nó
	//ai nele insiro o nó VERMELHO
	if(novo->info < no->info){
		no->esq = novo;
	}
	else {
		no->dir = novo;	
	}
	
	if(no->dir->cor == RED && no->dir->esq == BLACK)
		rot_esq(no);
	
	if(no->esq->cor == RED && no->esq->esq->cor == RED)
		rot_dir(no);
		
	if(no->esq->cor == RED && no->dir->cor == BLACK)
		flip_cor(no);
}

ArvoreRB * removerMenor(ArvoreRB * no){
	if(no->esq == NULL) {
		free(no);
		return NULL;
	}
	
	if(!eh_no_vermelho(no->esq) && !eh_no_vermelho(no->esq->esq))
		no = rot_esq(no);
	
	no->esq = removerMenor(no->esq);
	return no;
}

ArvoreRB * procurarMenor(ArvoreRB * no){
	ArvoreRB * no1 = no;
	ArvoreRB * no2 = no->esq;
	
	while(no2 != NULL){
		no1 = no2;
		no2 = no2->esq;
	}
	return no1;
}

ArvoreRB * remover_no(ArvoreRB * no, int valor) {
	if(valor < no->info) {
		if(!eh_no_vermelho(no->esq) && !eh_no_vermelho(no->esq->esq))
			no = rot_esq(no);
		no->esq = remover_no(no->esq, valor);
	} else {
		if(eh_no_vermelho(no->esq))
			no = rot_dir(no);
			
		if(valor == no->info && no->dir == NULL) {
			free(no);
			return NULL;
		}
		
		if(!eh_no_vermelho(no->dir) && !eh_no_vermelho(no->dir))
			no = rot_dir(no);	
		
		if(valor == no->info) {
			ArvoreRB * x = procurarMenor(no->dir);
			no->info = x->info;
			no->dir = removerMenor(no->dir);
		} else
			no->dir = remover_no(no->dir, valor);
	}
	
	return no;
}

ArvoreRB * criaFolha(int valor) {
	ArvoreRB* a = (ArvoreRB*)malloc(sizeof(ArvoreRB));
	a->info = valor;
	a->cor = RED;
	a->dir = NULL;
	a->esq = NULL;
	return a;
}

void remover(ArvoreRB * raiz, int valor){
	ArvoreRB * h = raiz;
	raiz = remover_no(h, valor);
	if(raiz != NULL)
		raiz->cor = BLACK;
}

int main(){
  ArvoreRB * a, * b, * c, * d, * e, * f;
  
  a = criaFolha(1);
  b = criaFolha(2);
  c = criaFolha(3);
  d = criaFolha(4);
  e = criaFolha(5);
  f = criaFolha(6);
  
  inserir(a, b);
  inserir(a, c);
  inserir(a, d);
  inserir(a, e);
  inserir(a, f);
  print(a, 3);

  
  remover(a, 3);
  remover(a, 5);
  print(a, 3);

}

