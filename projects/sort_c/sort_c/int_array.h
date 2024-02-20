#ifndef LAB1_INT_ARRAY_H
#define LAB1_INT_ARRAY_H

#include "array.h"


int *int_read_input(size_t *dimension);
void int_print_output(struct ArrayN_1 *output);
void int_free_struct(struct ArrayN_1 *a);

void *int_square(const void *x); 
int int_positive(const void *x);
struct ArrayN_1 *int_sort(struct ArrayN_1 *input);
#endif 
