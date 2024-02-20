#ifndef QUEUE_H
#define QUEUE_H

#include "item.h"
typedef struct Queue {
	Item *head;
	Item *tail;
} Queue;

Queue *queue_new();
void queue_delete(Queue *queue);
int queue_put(Queue *queue, char data);
int queue_get(Queue *queue, char *output);
void queue_print(const Queue *queue);

#endif
