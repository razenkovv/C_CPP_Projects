#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#define BUF 100

const char vowels[] = "AaEeIiOoUuYy";
const char consonants[] = "BbCcDdFfGgHhJjKkLlMmNnPpQqRrSsTtVvWwXxZz";

char *rm_marks(const char *s, int *count) {
	int i = 0, l = 0;
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


char **reshape_str(char *s, int *count) {
	*count = 0;
	char *res = NULL;
	res = rm_marks(s, count);
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


char ***reshape_data_line(char **text_array, int amount, int *lengths_of_lines) {
	char ***word_array = NULL;
	word_array = calloc(amount, sizeof(void *));
	for (int run = 0; run < amount; ++run) {
		int count;
		word_array[run] = reshape_str(text_array[run], &count);
		lengths_of_lines[run] = count;
	}
	return word_array;
}


char ***vowels_line(char **text_array, int amount, int *vowel_lengths_of_lines) {
	char ***vowel_array = NULL;
	vowel_array = calloc(amount, sizeof(void *));
	for (int run = 0; run < amount; ++run) {
		char **line_array = NULL;
		int count;
		line_array = reshape_str(text_array[run], &count);
		vowel_array[run] = calloc(count, sizeof(void *));
		int m = 0; //счетчик для массива гласных
		for (int j = 0; j < count; ++j) {
			if (strchr(vowels, line_array[j][0])) {
				++vowel_lengths_of_lines[run];
				vowel_array[run][m] = line_array[j];
				++m;
			} else {
				free(line_array[j]);;
			}
		}
		if (count > vowel_lengths_of_lines[run])
			vowel_array[run] = realloc(vowel_array[run], sizeof(void *) * vowel_lengths_of_lines[run]);
		free(line_array);
	}
	return vowel_array;
}


char ***cons_line(char **text_array, int amount, int *cons_lengths_of_lines) {
	char ***cons_array = NULL;
	cons_array = calloc(amount, sizeof(void *));
	for (int run = 0; run < amount; ++run) {
		char **line_array = NULL;
		int count;
		line_array = reshape_str(text_array[run], &count);
		cons_array[run] = calloc(count, sizeof(void *));
		int m = 0; //счетчик для массива согласных
		for (int j = 0; j < count; ++j) {
			if (strchr(consonants, line_array[j][0])) {
				++cons_lengths_of_lines[run];
				cons_array[run][m] = line_array[j];
				++m;
			} else {
				free(line_array[j]);;
			}
		}
		if (count > cons_lengths_of_lines[run])
			cons_array[run] = realloc(cons_array[run], sizeof(void *) * cons_lengths_of_lines[run]);
		free(line_array);
	}
	return cons_array;
}


char **reshape_data_all(char **text_array, int amount, int *total_word_amount) {
//исходные данные. количество строк, количество слов во всем тексте
	*total_word_amount = 0; //total_word_amount - количество слов во всём тексте
	char **word_array = NULL;
	int size_count = 1;
	word_array = calloc(BUF, sizeof(char *));
	for (int run = 0; run < amount; ++run) {   //amount - количество строк в тексте
		char **line_array = NULL;
		int count; //count - количество слов в одной текущей строке текста
		line_array = reshape_str(text_array[run], &count);
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


char **vowels_all(char **text_array, int amount, int *total_vowel_amount) {
	*total_vowel_amount = 0;
	char **vowel_array = NULL;
	int size_count = 1;
	vowel_array = calloc(BUF, sizeof(char *));
	for (int run = 0; run < amount; ++run) {
		char **line_array = NULL;
		int count;
		line_array = reshape_str(text_array[run], &count);
		for (int j = 0; j < count; ++j) {
			if (strchr(vowels, line_array[j][0])) {
				++*total_vowel_amount;
				if (*total_vowel_amount > (BUF * size_count)) {
					++size_count;
					vowel_array = realloc(vowel_array, sizeof(char *) * (BUF * size_count));
				}
				vowel_array[*total_vowel_amount - 1] = line_array[j];
			} else {
				free(line_array[j]);
			}
		}
		free(line_array);
	}
	if (*total_vowel_amount < (BUF * size_count))
		vowel_array = realloc(vowel_array, sizeof(char *) * *total_vowel_amount);
	return vowel_array;
}


char **cons_all(char **text_array, int amount, int *total_cons_amount) {
	*total_cons_amount = 0;
	char **cons_array = NULL;
	int size_count = 1;
	cons_array = calloc(BUF, sizeof(char *));
	for (int run = 0; run < amount; ++run) {
		char **line_array = NULL;
		int count;
		line_array = reshape_str(text_array[run], &count);
		for (int j = 0; j < count; ++j) {
			if (strchr(consonants, line_array[j][0])) {
				++*total_cons_amount;
				if (*total_cons_amount > (BUF * size_count)) {
					++size_count;
					cons_array = realloc(cons_array, sizeof(char *) * (BUF * size_count));
				}
				cons_array[*total_cons_amount - 1] = line_array[j];
			} else {
				free(line_array[j]);
			}
		}
		free(line_array);
	}
	if (*total_cons_amount < (BUF * size_count))
		cons_array = realloc(cons_array, sizeof(char *) * *total_cons_amount);
	return cons_array;
}


char **array_generate(int n2) {
	char **data = calloc(n2, sizeof(char *));
	for (int i = 0; i < n2; ++i) {
		srand(time(NULL) % (i*100 + 1));
		int k = rand()%15 + 1;
		data[i] = calloc((k+1), sizeof(char));
		for (int j = 0; j < k; ++j)
			data[i][j] = (char)(rand()%((int)'z' - (int)'a' + 1) + (int)'a'); 
		data[i][k] = '\0';
	}
return data;
}


void fprint_char_array(FILE *out, char **text_array, int amount) {
	for (int i = 0; i < amount; ++i) {
		fprintf(out, "%s\n", text_array[i]);
	}
}


void print_char_array(char **text_array, int amount) {
	for (int i = 0; i < amount; ++i) {
		printf("%s\n", text_array[i]);
	}
		printf("\n");
}


void clean_char_array(char **word_array, int amount) {
	for (int i = 0; i < amount; ++i) {
		free(word_array[i]);
	}
	free(word_array);
}


void fprint_arrays(FILE *out, char ***word, int amount, int *lengths) {
	for (int y = 0; y < amount; ++y) {
		for (int z = 0; z < lengths[y]; ++z) {
			fprintf(out, "%s ", word[y][z]);
		}
		fprintf(out, "\n");
	}
}


void print_arrays(char ***word, int amount, int *lengths) {
	for (int y = 0; y < amount; ++y) {
		for (int z = 0; z < lengths[y]; ++z) {
			printf("%s ", word[y][z]);
		}
		printf("\n");
	}
}


void clean_arrays(char ***word, int amount, int *lengths) {
	for (int y = 0; y < amount; ++y) {
		for (int z = 0; z < lengths[y]; ++z) {
			free(word[y][z]);
		}
		free(word[y]);
	}
	free(lengths);
	free(word);
}
