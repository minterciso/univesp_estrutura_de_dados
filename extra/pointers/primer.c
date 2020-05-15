#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void swap(int a, int b){
	printf("a=%d b=%d\n",a,b);
	int tmp = a;
	a=b;
	b=tmp;
	printf("a=%d b=%d\n",a,b);
}	

void swap2(int *a, int *b){
	printf("a=%d b=%d\n", *a, *b);
	int tmp=*a;
	*a=*b;
	*b=tmp;
	printf("a=%d b=%d\n", *a, *b);
}

int main(void){
	int a,b;
	a=10;
	b=20;
	printf("Main: a=%d b=%d\n", a,b);
	swap2(&a,&b);
	printf("Main: a=%d b=%d\n", a,b);
	return EXIT_SUCCESS;
}
