#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SExemplo{
	int id;
	char nome[80];
} REGISTRO;


void swap_full(REGISTRO *reg1, REGISTRO *reg2){
	REGISTRO tmp;
	size_t reg_size = sizeof(REGISTRO);
	memcpy(&tmp, reg1, reg_size);
	memcpy(reg1, reg2, reg_size);
	memcpy(reg2, &tmp, reg_size);
}

void swap_nome(REGISTRO *reg1, REGISTRO *reg2){
	REGISTRO tmp;
	strncpy(tmp.nome, reg2->nome, 80);
	strncpy(reg2->nome, reg1->nome, 80);
	strncpy(reg1->nome, tmp.nome, 80);
}

int main(void){
	REGISTRO reg1, reg2;
	reg1.id=0;
	strncpy(reg1.nome,"Asdrubal", 80);
	reg2.id=1;
	strncpy(reg2.nome,"Rocker", 80);

	printf("ID:%d\tNome:%s\n",reg1.id, reg1.nome);
	printf("ID:%d\tNome:%s\n",reg2.id, reg2.nome);
	swap_full(&reg1,&reg2);
	printf("ID:%d\tNome:%s\n",reg1.id, reg1.nome);
	printf("ID:%d\tNome:%s\n",reg2.id, reg2.nome);
	swap_full(&reg1,&reg2);
	swap_nome(&reg1, &reg2);
	printf("ID:%d\tNome:%s\n",reg1.id, reg1.nome);
	printf("ID:%d\tNome:%s\n",reg2.id, reg2.nome);
	return EXIT_SUCCESS;
}
