#ifndef LAB1_SORT_H
#define LAB1_SORT_H

void swap(void *b, void *e, size_t size);
void bubble_sort(void *base, size_t n, size_t size, int (*cmp) (const void *, const void *));
void insert_sort(void *base, size_t n, size_t size, int (*cmp) (const void *, const void *));
void quick_sort(void *base, size_t n, size_t size, int (*cmp) (const void *, const void *));

#endif //LAB1_SORT_H
