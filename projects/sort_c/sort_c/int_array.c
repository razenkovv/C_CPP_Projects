#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "array.h"
#include "int_array.h"
#include "sort.h"

#define BUF 10

/*int *int_read_input(size_t *dimension) {
    int *a = malloc(BUF * sizeof(int));
    int n = 0;
    size_t i = 0, k = 2;
    do {
	n = scanf("%d", a + i);
	if (n > 0) { 
	    if (i == BUF * (k - 1)) {
	       a = realloc(a, BUF * k * sizeof(int));
	       ++k;
	    }
	    ++i;
	}
    } while (n > 0);
    *dimension = i;
    return a;
}*/

int *int_read_input(size_t *dimension) {
    int n = 0;
    scanf("%d", &n);
    int *a = malloc(n * sizeof(int));
    for (int i = 0; i < n; ++i) {
	scanf("%d", a + i);
    }
    *dimension = n;
    return a;
}

void int_print_output(struct ArrayN_1 *output) {
    for (size_t i = 0; i < output->dimension; ++i) {
	int position = ((int *)output->list)[i];
	printf("%d ", position);
    }
    printf("\n");
}

void int_free_struct(struct ArrayN_1 *a) {
    free((int *)a->list);
    free(a);
}

void *int_square(const void *x) {
    int *y = malloc(1 * sizeof(int));
    *y = *(int *)x; 
    *y = *y * *y;
    return (void *)y;
}

int int_compare(const void *x1, const void *x2) {
    return (*(int *)x1 - *(int *)x2);
}

int int_positive(const void *x) {
    int *y = (int *)x;
    if (*y > 0)
       return 1;
    else return 0;
} 

struct ArrayN_1 *int_sort(struct ArrayN_1 *input) {
    struct ArrayN_1 *output = Create1(input->dimension, input->elementSize);
    memcpy(output->list, (const void *)input->list, input->dimension * input->elementSize);
    quick_sort(output->list, output->dimension, output->elementSize, int_compare);
    return output;
}
