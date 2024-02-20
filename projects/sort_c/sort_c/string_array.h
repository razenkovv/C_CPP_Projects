#ifndef LAB1_STRING_ARRAY_H
#define LAB1_STRING_ARRAY_H

#include "array.h"
char *string_get_str();
char **string_get_data(int *amount);
char *string_rm_marks(const char *s, int *count);
char **string_reshape_str(char *s, int *count);
char **string_reshape_data_all(char **text_array, int amount, int *total_word_amount);

int string_compare_alphabet(const void *x1, const void *x2);
int string_a_first(const void *x, size_t *l);
void *letter_up(const void *x, size_t *l);
void *remove_last_letter(const void *x, size_t *l);
void string_print_output(struct ArrayN_2 *output);
void string_free_struct(struct ArrayN_2 *a);
struct ArrayN_2 *string_sort(struct ArrayN_2 *input);

#endif
