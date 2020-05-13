# Introducao
Em C, uma coisas mais complexas é entender justamente o que são os ponteiros e como utilizar os mesmos. Linguagens mais atuais tendem a "esconder" essas informações do programador, o que em C não acontece e causa muita confusão. A ideia aqui é tentar facilitar um pouco a vida do estudante que não conhece muito C, para que seja capaz de enteder um básico de ponteiros, assim como utilizá-los.

# Conceito e sintaxe
O conceito de ponteiros é extramente simples, um ponteiro em C, literalmente __aponta__ para um endereço de memória, e nada mais. O que complica em C é a sintaxe do mesmo, temos duas sintaxes para utilizar ponteiros:

* __*__: Esse é o famoso ponteiro, ele é a representação de um endereço de memória.
* __&__: Esse é o __endereço de memória__ de uma variável.

Normalmente utilizamos _*_ quando precisamos utilizar o endereço de memória e _&_ quando precisamos passar o endereço de memória.

# Base
Em C, assim como outras linguagens, podemos passar parâmetros para uma função por __cópia__ ou por __referência__, veja a seguinte função abaixo:

```C
void swap(int a, int b){
    printf("a=%d b=%d\n", a, b);
    int tmp = a;
    a=b;
    b=tmp;
    printf("a=%d b=%d\n", a, b);
}
```

Nessa função estamos esperando dois parâmetros e estamos tentando trocar o parâmetro um com o outro, então vamos ver a execução do mesmo:
    
    a=10 b=20
    a=20 b=10

Fantástico, mas será que a troca foi feita mesmo? Vamos executar o programa completo com o código abaixo:

```C
#include <stdio.h>
#include <stdlib.h>

int main(void){
	int a,b;
	a=10;
	b=20;
	printf("Main: a=%d b=%d\n", a,b);
	swap(a,b);
	printf("Main: a=%d b=%d\n", a,b);
	return EXIT_SUCCESS;
}
```
E qual seria a saída?

    Main: a=10 b=20
    a=10 b=20
    a=20 b=10
    Main: a=10 b=20

Vamos ver o que aconteceu, vamos executar o código em um debugger. Para não complicar nem poluir o texto, vou mostrar apenas as partes que interessam:

    56	int main(void){
    (gdb) n
    67		a=10;
    (gdb) 
    68		b=20;
    (gdb) 
    69		printf("Main: a=%d b=%d\n", a,b);
    (gdb) p &a
    $2 = (int *) 0x7fffffffdfb8
    (gdb) p &b
    $3 = (int *) 0x7fffffffdfbc

Bom, como podemos utilizar _&_ para mostrar o endereço das variáveis, temos que as variáveis estão em 0x7fffffffdfb8 para _a_ e 0x7fffffffdfbc para _b_, vamos seguir com a execução quando entra na função `swap(int, int)`:

    (gdb) n
    Main: a=10 b=20
    70		swap(a,b);
    (gdb) s
    swap (a=21845, b=1431656109) at primer.c:48
    48	void swap(int a, int b){
    (gdb) n
    49		printf("a=%d b=%d\n",a,b);
    (gdb) p &a
    $4 = (int *) 0x7fffffffdf8c
    (gdb) p &b
    $5 = (int *) 0x7fffffffdf88

Opa, o endereço das variáveis é diferente, então quando a função `swap(int, int)` executa as instruções para efetuar o swap, ela executa com variáveis em locais diferentes de memória, então a troca funciona, mas fica apenas dentro do escopo da função. Isso é chamado de __passagem por cópia__, ou seja, por trás dos panos quando a função `swap(int, int)` é chamada, antes de executar as instruções o compilador copia o __valor__ das variáveis passadas para o endereço de memória interno das funções. O que precisamos é enviar o endereço de memória para efetuar o swap, logo vamos por partes.

## Assinatura da função
Vamos começar com a assinatura da função, essa mudança é a mais simples. Para manter histórico, vamos criar uma função chamada _swap2_. Para modificar a função _swap_ para receber ponteiros, basta alterar o _int_ para um __ponteiro do tipo int__, ou seja: `swap2(int *a, int *b)`.Isso indica para o compilador que a função _swap2_ recebe dois ponteiros do tipo __int__ (sim, ponteiros possuem vários tipos, vamos falar mais na frente isso). Como agora alteramos as entradas de __int__ para __ponteiro do tipo int__, precisamos alterar a chamada da função, para que ela consiga enviar o endereço de memória (lembra que o ponteiro aponta para um endereço de memória?), logo temos que a linha que antes era `swap(a,b);` agora ficaria `swap2(&a, &b);`.

## Código da função
Vamos ver como fica o novo código da função, é aqui que as coisas começam a complicar um pouco.

Recapitulando: _*_ é usado para __apontar__ para um endereço de memória e _&_ é utilizado para __encontrar__ o endereço de memória, então:

```C
void swap2(int *a, int *b){
    printf("a=%d b=%d\n", *a, *b);
    int tmp=*a;
    *a = *b;
    *b = tmp;
    printf("a=%d b=%d\n", *a, *b);
}
```

Vamos por partes. 

A linha `printf("a=%d b=%d\n", *a, *b);` precisa dos _*_ uma vez que a variável _a_ e _b_ é um endereço de memória, então se fossemos fazer dessa forma: `printf("a=%d b=%d\n",a,b);` estaríamos tentando mostrar o endereço de memória de a e b. Tudo bem, isso não é proibido, mas quando passamos %d para o printf() ele espera um valor de um __int__ e não um endereço de memória, logo teríamos um erro na hora de compilar:

    primer.c: In function ‘swap2’:
    primer.c:57:13: warning: format ‘%d’ expects argument of type ‘int’, but argument 2 has type ‘int *’ [-Wformat=]
       57 |  printf("a=%d b=%d\n", a, b);
          |            ~^          ~
          |             |          |
          |             int        int *
          |            %ls
    primer.c:57:18: warning: format ‘%d’ expects argument of type ‘int’, but argument 3 has type ‘int *’ [-Wformat=]
       57 |  printf("a=%d b=%d\n", a, b);
          |                 ~^        ~
          |                  |        |
          |                  int      int *
          |                 %ls

A linha `int tmp=*a;` está literalmente dizendo para o compilador o seguinte:

> Compilador, crie uma variável __local__ na minha função, chamada tmp, e copie o __valor__ do que está na memória __apontada__ na variável a nela.

A linha `*a=*b;` está copiando o valor da variável __apontada__ por b na variável __apontada__ por a. Aqui é importante falar sobre o que aconteceria se a linha fosse `a=b;` como anterirmente. Não se esqueçam que na função `swap2(int*, int*)`, a e b são __ponteiros__ para um endereço de memória, e ela foi chamada com `swap2(&a,&b);` dois endereços, como ela pede. Então ao fazer `a=b;` no lugar de `*a=*b;` estamos pedindo para o compilador colocar o __endereço__ de memória apontada por b em a, transformando então a e b no __exato__ mesmo endereço de memória! Ou seja, no lugar de copiarmos o __valor__ que possui na memória b para a, estamos falando essencialmente, que a __aponta__ para o mesmo local que b, que é um erro (e bem comum).

A linha `*b=tmp;` pede para copiar o valor da variável tmp (lembre, tmp é um __int__ e não um __int\*__) no endereço apontado por b. Que é exatamente o que queremos.

O código completo (com a função swap e swap2) fica dessa forma:

```C
#include <stdio.h>
#include <stdlib.h>

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

```
E a execução do mesmo:

    Main: a=10 b=20
    a=10 b=20
    a=20 b=10
    Main: a=20 b=10

Que é __exatamente__ o que queremos.
