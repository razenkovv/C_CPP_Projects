#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "array.h"
#include "string_array.h"
#include "sort.h"
#include <string.h>

#define BUF 10

char *string_get_str() {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do {
    	n = scanf("%80[^\n]", buf);
	if (n<0) {
	    if (!res) {
		return NULL;
	    }
	    } else if (n>0) {
		int chunk_len = strlen(buf);
		int str_len = len + chunk_len;
		res = (char*)realloc(res, str_len + 1);
		memcpy(res+len, buf, chunk_len);
		len = str_len;
	    } else {
		scanf("%*c");
	    }
	} while (n > 0);

    if (len > 0) {
	res[len] = '\0';
    } else {
	res = (char*)calloc(1, sizeof(char));
    }
    return res;
}

char **string_get_data(int *amount) {
    char **data = NULL;
    char *res = NULL;
    int  i = 0, j = 0;
    data = (char **) malloc(BUF * sizeof(char *));
    do {
	res = string_get_str();
	if (res) {
	    if ((i % BUF) == 0) {
		++j;
		data = (char **) realloc(data, sizeof(char *) * (BUF * j));	
	    }
	    data[i] = res;
	    ++i;
	}
    } while (res);
	
    if (i < (BUF * j))
	data = (char **) realloc(data, sizeof(char **) * i);
    *amount = i;
    return data;
}

char *string_rm_marks(const char *s, int *count) {
	int l = 0;
	char *res = NULL;
	res = malloc((strlen(s)+1) * sizeof(char));
	char *r = res;
	l = strspn(s, (", \"	';:-?!./()"));
	s += l;
	while (*s) {
		const char *s1 = s;
		l = strcspn(s1, (", \"	';:-?!./()"));
		s1 += l;
		++*count;
		memcpy(r, s, s1 - s);
		r += s1 - s;
		l = strspn(s1, (", \"	';:-?!./()"));
		s1 += l;
		if (*s1) {
			*r++ = (' ');
		}
		s = s1;
	}
	int len = r - res;
	res = realloc(res, len + 1);
	*(res + len) = ('\0');
	return (res);
}

char **string_reshape_str(char *s, int *count) {
	*count = 0;
	char *res = NULL;
	res = string_rm_marks(s, count);
	if (*count == 0) {
		free(res);
		return NULL;
	}
	int i = 0;
	char *k;
	char *run = res;
	char **word_array = NULL;
	word_array = (char **) calloc(*count, sizeof(char *));
	for (int m = 0; m < *count; ++m) {
		word_array[m] = NULL;
	}
	for (i; i < *count - 1; ++i) {
		k  = strchr(run, ' ');
		word_array[i] = (char *) malloc((k - run + 1) * sizeof(char));
		strncpy(word_array[i], run, k - run);
	    word_array[i][k - run] = '\0';
		run = k + 1;
	}
	word_array[i] = calloc((res + strlen(res) - run + 1), sizeof(char));
	strncpy(word_array[i], run, res + strlen(res) - run);
	word_array[i][res + strlen(res) - run] = '\0';
	free(res);
	return word_array;
}

char **string_reshape_data_all(char **text_array, int amount, int *total_word_amount) {
//исходные данные. количество строк, количество слов во всем тексте
	*total_word_amount = 0; //total_word_amount - количество слов во всём тексте
	char **word_array = NULL;
	int size_count = 1;
	word_array = calloc(BUF, sizeof(char *));
	for (int run = 0; run < amount; ++run) {   //amount - количество строк в тексте
		char **line_array = NULL;
		int count; //count - количество слов в одной текущей строке текста
		line_array = string_reshape_str(text_array[run], &count);
		for (int j = 0; j < count; ++j) {
			++*total_word_amount;
			if (*total_word_amount > (BUF * size_count)) {
				++size_count;
				word_array = realloc(word_array, sizeof(char *) * (BUF * size_count));
			}
			word_array[*total_word_amount - 1] = line_array[j];
		}
		free(line_array);
	}
	if (*total_word_amount < (BUF * size_count))
		word_array = realloc(word_array, sizeof(char *) * *total_word_amount);
	return word_array;
}

int string_compare_alphabet(const void *x1, const void *x2) {
	char *str1, *str2;
	str1 = *(char**) x1;
	str2 = *(char**) x2;
	int len1, len2, len_min;
	len1 = strlen(str1);
	len2 = strlen(str2);
	if (len1 < len2)
		len_min = len1;
	else len_min = len2;
	int diff = (int) ('a') - (int) ('A');
	int c1 = (int) *(str1);
	int c2 = (int) *(str2);
	if ((c1 >= (int) 'A') && (c1 <= (int) 'Z'))
		c1 +=diff;
	if ((c2 >= (int) 'A') && (c2 <= (int) 'Z'))
		c2 +=diff;

	if (c1 != c2)
		return c1 - c2;

	for (int i = 1; i < len_min; ++i) {
		int c1 = (int) *(str1 + i);
		int c2 = (int) *(str2 + i);
		if (c1 == c2)
			continue;

		return c1 - c2; 
	}
	return len1 - len2;
}

int string_a_first(const void *x, size_t *l) {
    char *y = (char *)x;
    if (!*l) 
	return 0;
    if (y[0] == 'a')
	return 1;
    else return 0;
}

void *letter_up(const void *x, size_t *l) {
    char *y = malloc(*l * sizeof(char));
    memcpy(y, (char *)x, *l * sizeof(char));
    int diff = 'A' - 'a';
    for (size_t i = 0; i < (*l - 1); ++i) {
	if ((y[i] >= 'a') && (y[i] <= 'z')) {
	    y[i] += diff;
	}
    }
    return (void *)y;
}

void *remove_last_letter(const void *x, size_t *l) {
    --*l;
    char *y = malloc(*l * sizeof(char));
    memcpy(y, (char *)x, *l * sizeof(char));
    y[*l-1] = '\0';
    return (void *)y;
}

void string_print_output(struct ArrayN_2 *output) {
    for (size_t i = 0; i < output->dimension; ++i) {
	char *position = ((char **)output->list)[i];
	printf("%s\n", position);
    }
}

void string_free_struct(struct ArrayN_2 *a) {
    for (size_t i = 0; i < a->dimension; ++i) {
	free((char *)a->list[i]);
    }
    free((char **)a->list);
    free(a->lengths);
    free(a);
}

struct ArrayN_2 *string_sort(struct ArrayN_2 *input) {
    struct ArrayN_2 *output = Create2(input->lengths, input->dimension, input->elementSize);
    for (size_t i = 0; i < input->dimension; ++i) 
	memcpy(output->list[i], input->list[i], input->lengths[i] * input->elementSize);
    quick_sort(output->list, output->dimension, sizeof(char *), string_compare_alphabet);
    return output;
}
