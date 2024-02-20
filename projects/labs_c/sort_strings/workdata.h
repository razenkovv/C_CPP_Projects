#ifndef WORKDATA_H
#define WORKDATA_H

char *rm_marks(const char *s, int *count);
char **reshape_str(char *s, int *count);
char ***reshape_data_line(char **text_array, int amount, int *lengths_of_lines);
char ***vowels_line(char **text_array, int amount, int *vowel_lengths_of_lines);
char ***cons_line(char **text_array, int amount, int *cons_lengths_of_lines);
char **reshape_data_all(char **text_array, int amount, int *total_word_amount);
char **vowels_all(char **text_array, int amount, int *total_vowel_amount);
char **cons_all(char **text_array, int amount, int *total_cons_amount);
char **array_generate(int n2);
void fprint_char_array(FILE *out, char **text_array, int amount);
void print_char_array(char **text_array, int amount);
void clean_char_array(char **word_array, int amount);
void fprint_arrays(FILE *out, char ***word, int amount, int *lengths);
void print_arrays(char ***words, int amount, int *lengths);
void clean_arrays(char ***words, int amount, int *lengths);

#endif
