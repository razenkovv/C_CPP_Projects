#include <stdlib.h>
#include "queue.h"

Queue *generate(int n) {
	Queue *q = queue_new();	
	int amount = rand() % (n/5) + 1;
	if (amount % 2)
		++amount;
	int *brackets = calloc(n, sizeof(int));
	int j = 0, i = 0;
	
	while (i < n / 2) {
		int k = rand() % 6;
		if (k == 5) {
			brackets[i] = 1;
			++j;
		}
		if (j == amount / 2)
			break;
		++i;
	}

	if (j < amount / 2)
		amount = j * 2;
	
	j = 0;
	i = n / 2;
	while (i < n) {
		int k = rand() % 6;
		if (k == 5) {
			brackets[i] = 2;
			++j;
		}
		if (j == amount / 2)
			break;
		++i;
	}
	
	i = n / 2;
	while (j < amount / 2) {
		if (brackets[i] != 2) {
			brackets[i] = 2;
			++j;
		}
		++i;
	}

	for (i = 0; i < n; ++i) {
		if (brackets[i] == 1)
			queue_put(q, '(');
		else if (brackets[i] == 2)
			queue_put(q, ')');
		else {
			char c = (char) (rand()%((int)'z' - (int)'a' + 1) + (int)'a');
			queue_put(q, c);
		}
	}
	free(brackets);
	return q;		
}

