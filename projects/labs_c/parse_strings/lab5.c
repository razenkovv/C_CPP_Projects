#include <stdio.h>
#include <stdlib.h>
#include "work.h"

int main() {
	printf("Hello. Enter formula:\n");
	int error = 0;
    Queue *q = queue_new();
	Stack *s = stack_new();
	char c = getchar();
	while (c != EOF) {
		if (c != '\n') {
			scan_and_check(c, q, s, &error);
		} else {
			printf("Input : \"");
			queue_print(q);
			printf("\"\n");
			if (!stack_empty(s))
				error = 1;
			if (error)
				printf("Incorrect input.\n");
			else {
				work(q);
				rm_spaces(q);
				printf("Output: \"");
				queue_print(q);
				printf("\"\n");
			}
			queue_delete(q);
	        stack_delete(s);
			q = queue_new();
			s = stack_new();
			error = 0;
		}
		c = getchar();
	}
		queue_delete(q);
		stack_delete(s);
return 0;
}
