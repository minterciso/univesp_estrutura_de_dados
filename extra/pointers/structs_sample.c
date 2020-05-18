#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct SExemplo{
	int id;
	char nome[80];
} REGISTRO;


int main(void){
	REGISTRO reg1;
	reg1.id=0;
	strncpy(reg1.nome,"Asdrubal", 80);

	printf("ID:%d\tNome:%s\n",reg1.id, reg1.nome);
	return EXIT_SUCCESS;
}
