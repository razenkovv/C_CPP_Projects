#ifndef STACK_H
#define STACK_H

#include "item.h"
typedef struct Stack {
	Item *head;
} Stack;

Stack *stack_new();
void stack_delete(Stack *stack);
int stack_push(Stack *stack, char data);
int stack_pop(Stack *stack, char *output);
void stack_print(const Stack *stack);
int stack_empty(const Stack *stack);

#endif
