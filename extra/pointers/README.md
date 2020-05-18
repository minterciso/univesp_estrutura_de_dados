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

# Estruturas
Lembra no começo que eu falei que existiam duas sintaxes? Pois bem, existe uma terceira exclusiva para estruturas:

* Operador seta: ->

Para entender esse operador, precisamos antes de mais nada entender (por cima) como funciona uma estrutura em C. Em C uma estrutura é definida pelo tipo `struct` e cria uma *"estrutura"* que pode ter 1 ou mais ítens diferentes.
Por exemplo:
```C
struct SExemplo{
	int id;
	char nome[80];
};
```

Para utilizarmos, podemos utilizar a seguinte `main()`:

```C
int main(void){
	struct SExemplo reg1;
	reg1.id=0;
	strncpy(reg1.nome,"Asdrubal", 80);

	printf("ID:%d\tNome:%s\n",reg1.id, reg1.nome);
	return EXIT_SUCCESS;
}
```

Pois bem, uma vez compilado, qual é a saída do programa?

> ID:0	Nome:Asdrubal

Como esperado, agora, o que acontece por trás? Vamos ver no debugger como ficam os endereços de memória, criando um breakpoint depois de popular a estrutura:

    18		printf("ID:%d\tNome:%s\n",reg1.id, reg1.nome);
    (gdb) p reg1
    $3 = {id = 0, nome = "Asdrubal", '\000' <repeats 71 times>}
    (gdb) p &reg1
    $4 = (struct SExemplo *) 0x7fffffffda00
    (gdb) p &reg1.id
    $5 = (int *) 0x7fffffffda00
    (gdb) p &reg1.nome
     $6 = (char (*)[80]) 0x7fffffffda04

Ou seja, o que podemos levar dessa história toda é que uma estrutura "junta" várias variáveis em um único ponto de entrada, em questão de endereço de memória. Dessa forma, quando fazemos uma alocação dinâmica, precisamos de uma nova sintaxe para avisar ao compilador que estamos falando de uma estrutura e não de uma variável única, por isso o operador ->.

Para simplificar, normalmente fazemos a seguinte linha extra:
```C
typedef struct SExemplo REGISTRO;
```

Ou juntamos tudo em:
```C
typedef struct SExemplo{
	int id;
	char nome[80];
} REGISTRO;
```

Dessa forma criamos o "tipo" novo chamado `REGISTRO`, que é uma simplificação do código `struct SExemplo`.

O mesmo código apresetando antes, agora com essa simplificação fica:
```C
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
```

## Ponteiros e Swap
Como estamos trabalhando com estruturas, vamos fazer dois métodos de `swap()`:

* `swap_full`: Faz swap do registro completo
* `swap_nome`: Faz swap apenas do nome

Para isso vamos precisar de duas funções de C, que estão no `string.h`, strncpy() e memcpy(). As duas copia os valores de endereço de memória de um ponteiro para outro, mas a primeira para quando encontra um '\0' e é mais otimizada para strings, a segunde é genérica.

Vamos ao código dos métodos:
```C
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
```

Um detalhe, vejam que existe a linha `size_t reg_size = sizeof(REGISTRO);`, isso é a mesma coisa que `size_t reg_size = sizeof(struct SExemplo)`, e basicamente cria uma variável chamada `reg_size` que possui o tamanho em bytes da estrutura SExemplo.

A função `swap_full` não tem nada de novo, ela apenas __copia__ reg\_size bytes do que está no endereço de memória __apontado__ por reg1 para tmp, depois reg\_size bytes do que aponta no reg2 para reg1, e depois reg\_size bytes de tmp para reg2. Mas onde entra o famoso '->'? Na função `swap_nome`.

Diferente da `swap_full`, a `swap_nome` queremos mudar __apenas__ o nome, mas manter o id da estrutura, logo precisamos conseguir encontrar o campo nome dentro do que é apontado por reg1 e reg2. É exatamente para isso que existe o operador '->', quando o compilador pega isso, ele entende que estamos buscando a variável dentro de um ponteiro de uma estrutura. Veja como exemplo o primeiro `strncpy(tmp.nome, reg2->nome, 80);`. Vamos destrinchar um pouco mais o que acontece aqui, veja que a assinatura da função strncpy() é:

```C
char *strncpy(char *dest, const char *src, size_t n);
```

Bom, ela precisa de dois ponteiros do tipo char e um valor com a quantidade de bytes (size\_t é um tipo de int) para copiar, porém quando passamos, passamos: `tmp.nome` como destino, e `reg2->nome` como origem, não deveria ser `&tmp.nome` e `&reg2->nome`?

Se você se lembrar, a nossa definição da estrutura diz que `nome` é `char nome[80];`, isso em C é um __ponteiro__ do tipo __char__ que possui 80 __chars__ alocados de maneira fixa (não dinâmica usando `malloc`), logo `tmp.nome` representa o __ponteiro__ para o __início__ da _string_ nome dentro da estrutura `tmp`, já `reg2->nome` é a mesma coisa, porém para `reg2` que é um __ponteiro__ para um estrutura do tipo SExemplo, chamada de `reg2`.

Pode parecer confuso, mas pense dessa forma:

* estou chamando a estrutura e não o ponteiro da estrutura? Uso '.' para acessar os campos.
* estou chamando o ponteiro do endereço de memória da estrutura? Uso '->' para acessar os campos.

Uma vez que cheguei no campo, tudo funciona normalmente.

Para quem tem curiosidade uma lista ligada pode ter esse tipo de estrutura:

```C
typedef struct node{
	int id;
	float grade;
	struct node *next;
} SNode;
```

Ou seja, estamos adicionando um campo, dentro da nossa estrutura chamada `node`, que aponta para uma outra estrutura, que eu ainda não sei onde está, chamada `next`, que por sua vez possui outro `next`, e outro, e outro...

Isso permite a seguinte linha:

```C
l->next->next->next->next->id;
```

Que basicamente diz que queremos acessar o campo `id` do quarto nó depois do nó apontado por `l`, caso ele exista obviamente. Mas isso é sobre lista ligada, e já não faz parte dessa explicação.
