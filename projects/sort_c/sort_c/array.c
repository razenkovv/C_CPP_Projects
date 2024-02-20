#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "array.h"
#include <stdio.h>

//для одномерных массивов
struct ArrayN_1 *Create1(size_t dimension, size_t elementSize) {
    struct ArrayN_1 *a = malloc(sizeof(struct ArrayN_1));
    a->dimension = dimension;
    a->elementSize = elementSize;
    a->list = malloc(elementSize * dimension);
    return a;
}

struct ArrayN_1 *AddToStruct1(void *array, size_t dimension, size_t elementSize) {
    struct ArrayN_1 *a = malloc(sizeof(struct ArrayN_1));
    a->dimension = dimension;
    a->elementSize = elementSize;
    a->list = array;
    return a;
}

struct ArrayN_1 *map1(struct ArrayN_1 *input, void *(*func) (const void *)) {
    struct ArrayN_1 *output = Create1(input->dimension, input->elementSize);
    for (size_t i = 0; i < input->dimension; ++i) {
	int8_t *position_input = (int8_t *)input->list + (i * input->elementSize);
	int8_t *position_output = (int8_t *)output->list + (i * output->elementSize);
	void *temp = func((const void *)position_input);
	memcpy(position_output, temp, input->elementSize);
	free(temp);
    }
    return output;
}

struct ArrayN_1 *where1(struct ArrayN_1 *input, int (*func) (const void *)) {
    struct ArrayN_1 *output = Create1(input->dimension, input->elementSize);
    size_t j = 0;
    for (size_t i = 0; i < input->dimension; ++i) {
	int8_t *position_input = (int8_t *)input->list + (i * input->elementSize);
	if (func((const void *)position_input)) {
	    int8_t *position_output = (int8_t *)output->list + (j * output->elementSize);
	    memcpy(position_output, position_input, input->elementSize);
	    ++j;
	}
    }
    output->list = realloc(output->list, j * output->elementSize);
    output->dimension = j;
    return output;
}

struct ArrayN_1 *concat1(struct ArrayN_1 *input1, struct ArrayN_1 *input2) {
    struct ArrayN_1 *output = Create1(input1->dimension + input2->dimension, input1->elementSize); 
    memcpy(output->list, input1->list, input1->dimension * input1->elementSize);
    void *pos1 = (int8_t *)output->list + input1->dimension * output->elementSize;
    void *pos2 = input2->list;
    memcpy(pos1, pos2, input2->dimension * input2->elementSize);
    return output;
}


//для двумерных массивов
struct ArrayN_2 *Create2(size_t *lengths, size_t dimension, size_t elementSize) {
    struct ArrayN_2 *a = malloc(sizeof(struct ArrayN_2));
    a->dimension = dimension;
    a->elementSize = elementSize;
    a->list = (void **)malloc(a->dimension * sizeof(void *));
    a->lengths = malloc(a->dimension * sizeof(size_t));
    for (size_t i = 0; i < a->dimension; ++i) {
	a->list[i] = malloc(a->elementSize * lengths[i]);
	a->lengths[i] = lengths[i];
    }	
    return a;
}

struct ArrayN_2 *AddToStruct2(void **array, size_t *lengths, size_t dimension, size_t elementSize) {
    struct ArrayN_2 *a = malloc(sizeof(struct ArrayN_2));
    a->dimension = dimension;
    a->elementSize = elementSize;
    a->list = array;
    a->lengths = lengths;
    return a;
}

struct ArrayN_2 *map2(struct ArrayN_2 *input, void *(*func) (const void *, size_t *)) {
    struct ArrayN_2 *output = malloc(sizeof(struct ArrayN_2));
    output->dimension = input->dimension;
    output->elementSize = input->elementSize;
    output->list = malloc(output->dimension * sizeof(void *));
    output->lengths = malloc(output->dimension * sizeof(size_t));
    for (size_t i = 0; i < input->dimension; ++i) {
	int8_t *position_input = ((int8_t **)input->list)[i];
	output->lengths[i] = input->lengths[i];
	void *temp = func((const void *)position_input, output->lengths + i);
	((int8_t **)output->list)[i] = (int8_t *)temp;
    }
    return output;
}

struct ArrayN_2 *where2(struct ArrayN_2 *input, int (*func) (const void *, size_t *)) {
    struct ArrayN_2 *output = malloc(sizeof(struct ArrayN_2));
    output->elementSize = input->elementSize;
    output->list = (void **)malloc(input->dimension * sizeof(void *));
    output->lengths = malloc(input->dimension * sizeof(size_t));
    size_t j = 0;
    for (size_t i = 0; i < input->dimension; ++i) {
	void *position_input = (input->list)[i];
	if (func((const void *)position_input, input->lengths + i)) {
	    output->list[j] = malloc(input->elementSize * input->lengths[i]);
	    output->lengths[j] = input->lengths[i];
	    memcpy((output->list)[j], position_input, input->lengths[i] * input->elementSize);
	    ++j;
	}
    }
    output->list = realloc(output->list, j * sizeof(void *));
    output->dimension = j;
    output->lengths = realloc(output->lengths, j * sizeof(size_t));
    return output;
}

struct ArrayN_2 *concat2(struct ArrayN_2 *input1, struct ArrayN_2 *input2) {
    struct ArrayN_2 *output = malloc(sizeof(struct ArrayN_2));
    output->dimension = input1->dimension + input2->dimension;
    output->elementSize = input1->elementSize;
    output->list = (void **)malloc((input1->dimension + input2->dimension) * sizeof(void *));
    output->lengths = malloc((input1->dimension + input2->dimension) * sizeof(size_t));
    for (size_t i = 0; i < input1->dimension; ++i) {
	void *position_input1 = (input1->list)[i];
	output->list[i] = malloc(input1->elementSize * input1->lengths[i]);
	output->lengths[i] = input1->lengths[i];
	memcpy((output->list)[i], position_input1, input1->lengths[i] * input1->elementSize);
    }
    for (size_t i = 0; i < input2->dimension; ++i) {
	void *position_input2 = (input2->list)[i];
	output->list[input1->dimension + i] = malloc(input2->elementSize * input2->lengths[i]);
	output->lengths[input1->dimension + i] = input2->lengths[i];
	memcpy((output->list)[input1->dimension + i], position_input2, input2->lengths[i] * input2->elementSize);
    }
    return output;
}
