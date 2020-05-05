#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/*
 * Implementação de Lista ligada dinâmica de acordo com https://www.youtube.com/watch?v=C6WOW0L1XO4&list=PLxI8Can9yAHf8k8LrUePyj0y3lLpigGcl&index=6
 */

typedef struct {
	int chave;
	float nota; // Adicionado mais para fazer sentido um "registro" do que apenas um inteiro
} REGISTRO;

// Essas duas estruturas poderia ser apenas uma
typedef struct aux {
	REGISTRO reg;
	struct aux *prox;
} ELEMENTO;

typedef struct {
	ELEMENTO* inicio;
} LISTA;

/**
 * @brief Inicializa a List ligada
 * @param l Um ponteiro para um endereço de memória do tipo LISTA previamente alocado
 */
void inicializarLista(LISTA *l)
{
	l->inicio = NULL;
}

/**
 * @brief Conta os elementos da list
 * @param l Um poteiro para uma lista previamente alocada
 * @return a quantidade de elementos
 */
int tamanho(LISTA *l)
{
	int tam = 0;
	ELEMENTO* end = l->inicio;
	while(end){
		tam++;
		end=end->prox;
	}
	return tam;
}

/**
 * @brief Exibe a lista de uma forma mais "bonita"
 * @param l Um ponteiro para uma lista previamente alocada
 */
void exibirLista(LISTA *l)
{
	ELEMENTO *end = l->inicio;
	fprintf(stdout,"Lista:\"");
	while(end){
		fprintf(stdout," %d: %.2f", end->reg.chave, end->reg.nota);
		end=end->prox;
	}
	fprintf(stdout,"\"\n");
}

/**
 * @brief Faz uma busca sequencial na lista l pela chave ch
 * @param l A lista inicialziada para efetuar a busca
 * @param ch A chave para buscar na lista
 * @return Um ponteiro para o elemento encontrado, ou NULL caso não encontre
 */
ELEMENTO* buscaSequencial(LISTA* l, int ch)
{
	ELEMENTO* pos = l->inicio;
	while(pos){
		if(pos->reg.chave == ch) return pos;
		pos=pos->prox;
	}
	return NULL;
}

/**
 * @brief Faz uma busca sequencial na lista l ordenada, pela chave ch
 * @param l A lista inicialziada para efetuar a busca
 * @param ch A chave para buscar na lista
 * @return Um ponteiro para o elemento encontrado, ou NULL caso não encontre
 */
ELEMENTO* buscaOrdenada(LISTA *l, int ch)
{
	ELEMENTO* pos = l->inicio;
	while(pos && pos->reg.chave < ch) pos=pos->prox;
	if(pos && pos->reg.chave == ch) return pos;
	return NULL;
}

/**
 * @brief Efetua uma busca sequencial por um elemento e armazena o endereço de memória do mesmo em ant
 * @param l A lista inicializada para efetuar a busca
 * @param ch A chave para buscar na lista
 * @param ant Um endereço de memória para armazenar o elemento anterior (se existir)
 * @return Um ponteiro para o elemento encontrado
 */
ELEMENTO* buscaSequencialExc(LISTA* l, int ch, ELEMENTO** ant)
{
	*ant = NULL; //Limpa o valor apontado por ant
	ELEMENTO* atual = l->inicio;
	while(atual && atual->reg.chave < ch){
		*ant = atual;
		atual=atual->prox;
	}
	if(atual && atual->reg.chave == ch) return atual;
	return NULL;
}

/**
 * @brief Insere de forma ordenada (e não repetida) o registro reg na lista l
 * @param l A lista inicializada
 * @param reg Um registro válido
 * @return 0 caso ocorra alguma falha, 1 caso contrário
 */
int inserirElemListaOrd(LISTA* l, REGISTRO reg)
{
	int ch = reg.chave;
	ELEMENTO* ant, *i;
	i = buscaSequencialExc(l, ch, &ant);
	if (i != NULL) return 0; // Caso exista i, retorna 0, uma vez que não podemos inserir elementos repetidos
	//Aloca memória para o novo elemento
	if((i=(ELEMENTO*)malloc(sizeof(ELEMENTO)))==NULL){
		perror("malloc");
		return 0;
	}
	i->reg=reg;
	if(ant == NULL){
		i->prox = l->inicio;
		l->inicio = i;
	}
	else{
		i->prox = ant->prox;
		ant->prox = i;
	}
	return 1;
}

/**
 * @brief Exclui o registro com a chave ch da lista l
 * @param l A lista inicializada
 * @param ch A chave para buscar e excluir
 * @return 0 caso ocorra alguma falha, 1 caso contrário
 */
int excluirElemLista(LISTA* l, int ch)
{
	ELEMENTO* ant, *i;
	i=buscaSequencialExc(l, ch, &ant);
	if(i==NULL) return 0; //Não existe o elemento, então retorna 0
	if(ant ==NULL) l->inicio = i->prox;
	else ant->prox = i->prox;
	free(i);
	return 1;
}

/**
 * @brief Limpa memória e reinicializa a lista
 * @param l Uma lista previamente alocada
 */
void reinicializarLista(LISTA *l)
{
	ELEMENTO* end = l->inicio;
	while(end){
		ELEMENTO* apagar = end;
		end = end->prox;
		free(apagar);
	}
	l->inicio = NULL;
}

int main(int argc, char *argv[]){
	int tam_lista = 10; // Tamanho padrao da lista
	LISTA l;
	REGISTRO tmp;
	int chave=0;

	// Pega a quantidade de valores a serem inseridos
	if(argc == 2)
		tam_lista = atoi(argv[1]);

	// Inicializa o nosso gerador de numeros aleatorios
	srand(time(NULL));

	fprintf(stdout, "Inserindo %d elementos na lista...\n", tam_lista);
	inicializarLista(&l);
	while(tamanho(&l) != tam_lista){
		tmp.chave = chave;
		tmp.nota = (float)rand()/(float)(RAND_MAX)*10;
		if(inserirElemListaOrd(&l, tmp) > 0)
			chave++;
	}
	exibirLista(&l);
	reinicializarLista(&l);

	return EXIT_SUCCESS;
}
