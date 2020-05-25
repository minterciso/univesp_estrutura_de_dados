#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>

// This is just the element part of the stack, each node will have one element that will be copyied to
// the stack.
typedef struct {
	int id;
	float grade;
}SElement;

// The actuall stack, you can think of this as a LIFO linked list, the "top" always has a pointer to the
// previous node, if it's NULL, then this is the first node
typedef struct stack{
	struct stack *prev;
	SElement el;
}SStack;

/**
 * @brief Create a stack node and return a pointer to the newly allocated memory
 * @return A pointer to a new stack node
 */
SStack* createStackNode(void)
{
	size_t stack_size = sizeof(SStack);
	SStack *stack = NULL;
	if((stack=(SStack*)malloc(stack_size))==NULL){
		perror("malloc");
		return NULL;
	}
	memset(stack,'\0',stack_size);
	return stack;
}

/**
 * @brief Push an element to the stack, and return the top of the stack
 * @param el The element to copy to the stack
 * @param stack The previous stack, if exists
 * @return A pointer to the top node
 */
SStack* push(SElement el, SStack *stack){
	SStack *top = NULL;
	SStack *new_node = NULL;

	// If stack is NULL, create the first node and set the pointer correctly
	if(!stack){
		stack = createStackNode();
		memcpy(&stack->el, &el, sizeof(SElement));
		stack->prev = NULL; // And there's no previous element
		return stack;
	}
	// If this is not the first element, find the first element
	new_node = createStackNode();
	memcpy(&new_node->el, &el, sizeof(SElement));
	new_node->prev = stack;

	return new_node;
}

/**
 * @brief Removes the top paramenter, store it on ret_element and free the memory, return a pointer to the top stack
 * @param stack The stack to remove from
 * @param ret_element Where to store the removed (poped) element
 * @return A pointer to the new top of the stack
 */
SStack* pop(SStack *stack, SElement *ret_element){
	if(!stack)
		return NULL;
	memcpy(ret_element, &stack->el, sizeof(SElement));

	// Now that we have stored the stack element to return, free the memory
	if(stack->prev == NULL){
		free(stack);
		return NULL;
	}
	SStack *node_to_free = stack;
	stack = stack->prev;
	free(node_to_free);
	return stack;
}

int main(int argc, char *argv[]){
	SStack *stack = NULL;
	SElement element;
	SElement ret_element;
	int num_elements;

	if(argc != 2)
		num_elements = 10;
	else
		num_elements = atoi(argv[1]);

	srand(time(NULL));

	printf("Inserting...\n");
	for(int i=0;i<num_elements;i++){
		element.id = i;
		element.grade = (float)(rand()/(float)RAND_MAX*10);
		printf(" -> ID=%d\tGrade=%.2f\n",element.id, element.grade);
		stack = push(element, stack);
	}
	printf("Removing...\n");
	for(int i=0;i<num_elements;i++){
		stack=pop(stack, &ret_element);
		printf("Element id: %d\tGrade:%.2f\n", ret_element.id, ret_element.grade);
	}
	return EXIT_SUCCESS;
}
