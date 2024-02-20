#include <stdlib.h>
#include <stdio.h>
#include "item.h"
#include "queue.h"

Queue *queue_new() {
    return (Queue *) calloc(1, sizeof(Queue));
}

void queue_delete(Queue *queue) {
    Item *ptr = queue->head, *ptr_prev;
    while (ptr) {
        ptr_prev = ptr;
        ptr = ptr->next;
        free(ptr_prev);
    }
	free(queue);
}

int queue_put(Queue *queue, pos data) {
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

int queue_get(Queue *queue) {
    if (!queue->head) {
    return -1;
    }
	if (queue->head == queue->tail) {
		queue->tail = NULL;
	}
	Item *head = queue->head;
	queue->head = head->next;
	free(head);
	return 0;
}

int queue_remove(Queue *queue, Item **ptr, Item **prev_ptr) {
	if (!queue->head) {
		return -1;
	}
	if (*ptr == queue->head) {
		queue_get(queue);
		*ptr = queue->head;
		*prev_ptr = queue->head;
	} else {
		(*prev_ptr)->next = (*ptr)->next;
		free(*ptr);
		*ptr = (*prev_ptr)->next;
		if (!(*ptr))
			queue->tail = *prev_ptr;
	}
	return 0;
}
