#include <stdlib.h>
#include "stack.h"
#include "queue.h"

void work(Queue *q) {
	if (q->head) {
		Item *run = q->head;
		Item *op = q->head;
		Item *tmp = op;
		while (run->next) {
			if (run->next->data == '(') {
				op = run;
				run = run->next;
			}
			else if ((run->next->data == ')') && (op)) {
				if ((op == q->head) && (op->data == '(')) {
					op = op->next;
					run = run->next;
					while (op != run) {
					//while (op->data != ')') {
						Item *prev = op;
						op = op->next;
						free(prev);
					}
					Item *prev = q->head;
					q->head = op;
					free(prev);
					run = op;
				} else {
					tmp = op;
					op = op->next;
					run = run->next;
					while (op != run) {
					//while (op->data != ')') {
						Item *prev = op;
						op = op->next;
						free(prev);
					}
					tmp->next = op->next;
					free(op);
					op = NULL;
					run = tmp;
			  	}
			} else 
				run = run->next;
		}
	
		if (q->head->data == ')') {
			Item *prev = q->head;
			q->head = q->head->next;
			free(prev);
		}
	}	
}


void rm_spaces(Queue *q) {
	if (q->head) {
		Item *run = q->head;
		while ((run->data == ' ') || (run->data == '	')) {
			Item *prev = run;
			run = run->next;
			free(prev);
			if (!run)
				break;
		}			
		q->head = run;
		if ((q->head) && (q->head->next)) {
			Item *tmp = q->head;
			while (run->next) {
				if (run->next->data == '	')
					run->next->data = ' ';

				if (run->next->data == ' ') {
					tmp = run;
					run = run->next;
					while (run->next) {
						if (run->next->data == '	')
							run->next->data = ' ';
						if (run->next->data == ' ') {
							Item *prev = run;
							run = run->next;
							free(prev);
						}
						else
							break;
					}
					tmp->next = run;
					if (run->next)
						run = run->next;
				} else
					run = run->next;
			}
			
			if (((tmp->next->data) == ' ') && (!tmp->next->next)) {
				free(tmp->next);
				tmp->next = NULL;
				q->tail = tmp;
			}
		}
	}
}

void scan_and_check(char c, Queue *q, Stack *s, int *error) {
	queue_put(q, c);
	if (!*error) {
		switch (c) {
		case '(': {
			stack_push(s, c);
			break;
		}
		case '[': {
			stack_push(s, c);
			break;
		}
		case ')': {
			char output;
			int check = stack_pop(s, &output);
			if (!check) {
				if ((output != '('))
						*error = 1;
			} else 
				*error = 1;
				break;
		}
		case ']': {
			char output;
			int check = stack_pop(s, &output);
			if (!check) {
				if (output != '[')
					*error = 1;
			} else 
				*error = 1;
				break;
		}
		}
	}
}

