#ifndef QUEUE_H
#define QUEUE_H

#include "item.h"
typedef struct Queue {
	Item *head;
	Item *tail;
} Queue;

Queue *queue_new();
void queue_delete(Queue *queue);
int queue_put(Queue *queue, pos data);
int queue_get(Queue *queue);
int queue_remove(Queue *queue, Item **ptr, Item **ptr_prev);

#endif
