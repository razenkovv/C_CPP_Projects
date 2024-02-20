#ifndef LAB1_ARRAY_H
#define LAB1_ARRAY_H

#include <stddef.h>
//для одномерных массивов
struct ArrayN_1 {
    size_t dimension;
    size_t elementSize;
    void *list;
};

//для двумерных массивов
struct ArrayN_2 {
    size_t dimension;
    size_t elementSize;
    void **list;
    size_t *lengths;
};

struct ArrayN_1 *Create1(size_t dimension, size_t elementSize);
struct ArrayN_1 *AddToStruct1(void *array, size_t dimension, size_t elementSize);
struct ArrayN_1 *map1(struct ArrayN_1 *input, void *(*func) (const void *));
struct ArrayN_1 *where1(struct ArrayN_1 *input, int (*func) (const void *));
struct ArrayN_1 *concat1(struct ArrayN_1 *input1, struct ArrayN_1 *input2);

struct ArrayN_2 *Create2(size_t *lengths, size_t dimension, size_t elementSize);
struct ArrayN_2 *AddToStruct2(void **array, size_t *lengths, size_t dimension, size_t elementSize);
struct ArrayN_2 *map2(struct ArrayN_2 *input, void *(*func) (const void *, size_t *));
struct ArrayN_2 *where2(struct ArrayN_2 *input, int (*func) (const void *, size_t *));
struct ArrayN_2 *concat2(struct ArrayN_2 *input1, struct ArrayN_2 *input2);

#endif
