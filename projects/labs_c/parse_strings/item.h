#ifndef ITEM_H
#define ITEM_H

typedef struct Item {
	char data;
	struct Item *next;
} Item;

#endif
