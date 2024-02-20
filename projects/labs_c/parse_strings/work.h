#ifndef WORK_H
#define WORK_H

#include "queue.h"
#include "stack.h"

void work(Queue *q);
void rm_spaces(Queue *q);
void scan_and_check(char c, Queue *q, Stack *s, int *error);

#endif
