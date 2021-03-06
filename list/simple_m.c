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
 * @brief Search on an unordered list and return the Node found, if any
 * @param l An already allocated linked list
 * @param id The id to search for
 * @return A pointer to the node found, or NULL if not found
 */
SList* search(SList *l, int id){
	SList *node = l;
	while(node && node->reg.id < id) node=node->next;
	if(node && node->reg.id == id) return node;
	return NULL;
}

/**
 * @brief Search for the id on the list, and store the previous (if any) on the pointer to prev
 * @param l An already allocated linked list
 * @param id The id to search for
 * @param prev A pointer to a Node to store the previous, if any
 * @return The node found, if any
 */
SList* searchWithPrev(SList *l, int id, SList **prev){
	SList *node = l;
	while(node && node->reg.id < id){
		*prev = node;
		node=node->next;
	}
	if(node && node->reg.id == id) return node;
	return NULL;
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
 * @brief Add an ordered node
 * @param l An already initiated linked list
 * @param reg The registry to add
 * @return 0 if there's any error, 1 otherwise
 */
int addOrderedNode(SList *l, SRegistry reg){
	int id = reg.id;
	SList *prev_node = NULL, *node = NULL;

	node = searchWithPrev(l, id, &prev_node);
	if(node != NULL) return 0;

	node = createNode(&reg);
	if(prev_node == NULL){
		node->next = l;
		l=node;
	}
	else{
		node->next=prev_node->next;
		prev_node->next=node;
	}

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
		tmp_registry.id=rand();
		tmp_registry.grade=(float)rand()/(float)(RAND_MAX)*10;
		if(l==NULL)
			l=createNode(&tmp_registry);
		else
			addOrderedNode(l,tmp_registry);
		curr_length = length(l);
	}
	print(l);
	clearList(l);

	return EXIT_SUCCESS;
}

