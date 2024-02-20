#include <stdlib.h>
#include <stdio.h>
#include "item.h"
#include "queue.h"

Queue *queue_new() {
    return (Queue *) calloc(1, sizeof(Queue));
}

void queue_delete (Queue *queue) {
    Item *ptr = queue->head, *ptr_prev;
    while (ptr) {
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
    free(queue);
}

int queue_put(Queue *queue, char data) {
    Item *new = (Item *) malloc(sizeof(Item));
    if (!new) {
	    return -1;
	}
	new->data = data;
    new->next = NULL;
    if (!queue->head) {
		queue->head = new;
		queue->tail = new;
	} else {
		queue->tail->next = new;
		queue->tail = new;
    }
	return 0;
}

int queue_get(Queue *queue, char *output) {
    if (!queue->head) {
    return -1;
    }
    *output = queue->head->data;
	if (queue->head == queue->tail) {
		queue->tail = NULL;
	}
	Item *head = queue->head;
	queue->head = head->next;
	free(head);
	return 0;
}

void queue_print(const Queue *queue) {
	Item *ptr = queue->head;
	while (ptr) {
		printf("%c", ptr->data);
	    ptr = ptr->next;
    }
}
