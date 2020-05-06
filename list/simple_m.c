#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

/*
 * Adaptacao da lista ligada apresentada em https://www.youtube.com/watch?v=C6WOW0L1XO4&list=PLxI8Can9yAHf8k8LrUePyj0y3lLpigGcl&index=6
 * As maiores diferenças são:
 * - Junção de duas estruturas em uma (na versão original, existe uma estrutura apenas para mostrar o início da lista)
 * - Utilização da língua inglesa para nomenclaturas e documentação
 */

typedef struct {
	int id;
	float grade;
} SRegistry;

typedef struct s_linked_list{
	SRegistry reg;
	struct s_linked_list *next;
} SList;

/**
 * @brief Creates the memory for one node of the linked list (empty node) and return the memory pointer.
 * @param with_reg A pointer to a registry, if it's not NULL, it'll copy to the node, otherwise it'll start the registry as 0.
 * @return A pointer to a valid Linked List
 */
SList *createNode(SRegistry *with_reg){
	SRegistry reg;
	size_t node_size = sizeof(SList);
	SList *l; // The pointer to the linke list that'll be returned

	if(with_reg != NULL){
		reg.id = with_reg->id;
		reg.grade = with_reg->grade;
	}
	else{
		reg.id=0;
		reg.grade=0.0;
	}

	if((l=(SList*)malloc(node_size))==NULL){
		perror("malloc");
		return NULL;
	}
	l->reg = reg;
	l->next = NULL;
	return l;
}

/**
 * @brief Returns the linked list length
 * @param l An already allocated linked list
 * @return The length (amount of nodes) of the list, or -1 if the list is invalid
 */
int length(SList *l)
{
	SList *node = l;
	int size=0;

	if(node == NULL)
		return -1;
	while(node){
		size++;
		node=node->next;
	}
	return size;
}

/**
 * @brief Pretty print the linked list
 * @param l An already allocated linked list
 */
void print(SList *l)
{
	SList *node = l;

	fprintf(stdout,"List:");
	if(node == NULL){
		fprintf(stdout," INVALID LIST\n");
		return;
	}
	while(node){
		fprintf(stdout,"[%d: %.2f] ", node->reg.id, node->reg.grade);
		node=node->next;
	}
	fprintf(stdout,"\n");
}

/**
 * @brief Add an UNORDERED registry on the linked list
 * @param l An already allocated linked list
 * @param reg The registry to add to the linke list
 * @return 0 in case of failure, 1 otherwise
 */
int addNode(SList *l, SRegistry reg){
	SList *new_node = NULL, *node = NULL;
	new_node = createNode(&reg);
	if(new_node==NULL)
		return 0;
	new_node->reg = reg;
	new_node->next = NULL;
	node=l;
	while(node->next)
		node=node->next;
	node->next=new_node;
	return 1;
}

/**
 * @brief Clear the memory used for the linked list.
 * @param l An already initiaded linked list
 */
void clearList(SList *l)
{
	SList *node = l, *tmp = NULL;
	while(node){
		tmp=node;
		node=node->next;
		free(tmp);
	}
	if(node)
		free(node);
}

int main(int argc, char *argv[])
{
	int list_length = 10, curr_length=0;
	SList *l = NULL;
	SRegistry tmp_registry;
	int id = 0;
	
	if(argc == 2)
		list_length = atoi(argv[1]);

	srand(time(NULL));

	fprintf(stdout,"Inserting %d random elements on the list...\n", list_length);
	while(curr_length != list_length){
		tmp_registry.id=id++;
		tmp_registry.grade=(float)rand()/(float)(RAND_MAX)*10;
		if(l==NULL)
			l=createNode(&tmp_registry);
		else{
			if(addNode(l,tmp_registry) <= 0){
				fprintf(stderr,"Error adding to list\n");
				return EXIT_FAILURE;
			}
		}
		curr_length = length(l);
	}
	print(l);
	clearList(l);

	return EXIT_SUCCESS;
}

/*
typedef struct s_list{
	int elem;
	struct s_list *next;
}SList;

SList* create_node(int elem){
	ssize_t list_size = sizeof(SList);
	SList *p = NULL;

	if((p=(SList*)malloc(list_size))==NULL){
		perror("malloc");
		return NULL;
	}
	memset(p, '\0', list_size);
	p->elem = elem;
	return p;
}

void print(SList *list){
	SList *p = list;
	fprintf(stdout,"List:");
	while(p){
		fprintf(stdout," %d", p->elem);
		p=p->next;
	}
	fprintf(stdout,"\n");
}

SList* add(SList *list, int elem){
	SList *node = create_node(elem);
	SList *p = NULL;

	if(node == NULL){
		fprintf(stderr,"Unable to add node to list.\n");
		return NULL;
	}
	if(list == NULL)
		return node;
	p=list;
	while(p->next)
		p=p->next;
	p->next = node;
	return list;
}

void clear(SList *list){
	SList *p = list;
	SList *rem = NULL;
	
	while(p->next){
		rem=p;
		p=p->next;
		free(rem);
	}
	if(p)
		free(p);
}

int main(void){
	return EXIT_SUCCESS;
}
*/
