#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "sort.h"
#include "getdata.h"
#include "workdata.h"
#include "cmp.h"


void clean_all(char **text, char **word, char **vowel_all, char **conss_all, char ***word_line, char ***vowel_line, char ***conss_line, int *lengths_of_lines, int *lengths_of_vowels, int *lengths_of_cons, int amount, int total_word_amount, int total_vowel_amount, int total_cons_amount) {
	if (text)
		clean_char_array(text, amount);
	if (word)
		clean_char_array(word, total_word_amount);
	if (vowel_all)
		clean_char_array(vowel_all, total_vowel_amount);
	if (conss_all)
		clean_char_array(conss_all, total_cons_amount);
	if (word_line)
		clean_arrays(word_line, amount, lengths_of_lines);
	if (vowel_line)
		clean_arrays(vowel_line, amount, lengths_of_vowels);
	if (conss_line)
		clean_arrays(conss_line, amount, lengths_of_cons);
}


int main() {

char **text = NULL; //массив в котором хранится введенный текст(либо с клавиатуры, либо из файла)
char **word = NULL; //массив, в котором хранится текст, разбитый на отдельные слова
char **vowel_all = NULL; //массив, в котором хранятся слова, начинающиеся на гласные
char **conss_all = NULL; //массив, в котором хранятся слова, начинающиеся на согласные
char ***word_line = NULL; //массив массивов. исходный текст разбивается на строки, каждая строка является массивом слов
char ***vowel_line = NULL; //массив массивов. содержит только слова, начинающиеся на гласные
char ***conss_line = NULL; //массив массивов. содержит только слова, начинающиеся на согласные
int amount = 0; //количество строк в тексте
int total_word_amount = 0; //общее количество слов во всем тексте
int total_vowel_amount = 0; //общее количество слов, начинающихся на гласные
int total_cons_amount = 0; //общее количество слов, начинающихся на согласные
int *lengths_of_lines = NULL; //массив, в котором хранятся количества слов в каждой строке
int *lengths_of_vowels = NULL; // массив, в котором хранятся количества слов, наичнающихся на гласные, в каждой строке
int *lengths_of_cons = NULL; // массив, в котором хранятся количества слов, наичнающихся на согласные, в каждой строке
FILE *in = NULL;
FILE *out = NULL; //файлы


while (1) {

	printf("1 - read text from file (all data will be overwritten)\n2 - enter text (all data will be overwritten)\n3 - check time\n4 - work with text\n5 - save text in file\n6 - print text on the screen\n7 - exit\n");
	int main_choice = scan_choice(7);

	switch (main_choice) {
	case 1: {
		clean_all(text, word, vowel_all, conss_all, word_line, vowel_line, conss_line, lengths_of_lines, lengths_of_vowels, lengths_of_cons, amount, total_word_amount, total_vowel_amount, total_cons_amount); 
		in = open_file_in();
		text = fget_data(in, &amount);
		printf("Data written succesfully.\n\n");
		fclose(in);
		break;
	}
	case 2: {
		clean_all(text, word, vowel_all, conss_all, word_line, vowel_line, conss_line, lengths_of_lines, lengths_of_vowels, lengths_of_cons, amount, total_word_amount, total_vowel_amount, total_cons_amount); 
		text = get_data(&amount);
		printf("Data written succesfully.\n\n");
		break;
	}
	case 3: {
		int n1 = 0, n2 = 0;
		printf("Enter amount of arrays: ");
		while (scanf("%d", &n1) != 1) {
			while (getchar() != '\n') {};
			printf("Please reenter: ");
		}
		while (getchar() != '\n') {};
		printf("Enter dimension of array: ");
		while (scanf("%d", &n2) != 1) {
			while (getchar() != '\n') {};
			printf("Please reenter: ");
		}
		while (getchar() != '\n') {};
		printf("Please choose the sort:\n1 - qsort (library function)\n2 - quick_sort\n3 - insert_sort\n4 - bubble_sort\n");
		int sorting_choice = scan_choice(4);
		double time_total = 0;
		for (int r = 0; r < n1; ++r) {
			char **data = NULL;
			switch (sorting_choice) {
			case 1: {
				data = array_generate(n2);
				clock_t t = clock();
				qsort(data, n2, sizeof(char *), compare_alphabet);
				t = clock() - t;
				double time = (double) t / CLOCKS_PER_SEC;
				time_total+=time;
				break;
			}
			case 2: {
				data = array_generate(n2);
				clock_t t = clock();
				quick_sort(data, n2, sizeof(char *), compare_alphabet);
				t = clock() - t;
				double time = (double) t / CLOCKS_PER_SEC;
				time_total+=time;
				break;
			}
			case 3: {
				data = array_generate(n2);
				clock_t t = clock();
				insert_sort(data, n2, sizeof(char *), compare_alphabet);
				t = clock() - t;
				double time = (double) t / CLOCKS_PER_SEC;
				time_total+=time;
				break;
			}
			case 4: {
				data = array_generate(n2);
				clock_t t = clock();
				bubble_sort(data, n2, sizeof(char *), compare_alphabet);
				t = clock() - t;
				double time = (double) t / CLOCKS_PER_SEC;
				time_total+=time;
				break;
			}
			}
			clean_char_array(data, n2);
		}
		printf("Average time: %.8lf seconds.\n", time_total / n1); 
		break;
	}
	case 4: {
		if (text == NULL)
			printf("You haven't entered anything.\n\n");
		else {
			while (1) {
			printf("1 - sort all text in alphabet order\n2 - sort each line in alphabet order\n3 - find words starting with vowels\n4 - find words starting with consonants\n5 - back\n");
			int work_choice = scan_choice(5);

			switch (work_choice) {
			case 1: {
				if (word)
					clean_char_array(word, total_word_amount);
				word = reshape_data_all(text, amount, &total_word_amount);
				quick_sort(word, total_word_amount, sizeof(char *), compare_alphabet);
				printf("Sorted succesfully.\n\n");
				break;
			}
			case 2: {
				if (word_line)
					clean_arrays(word_line, amount, lengths_of_lines);
				lengths_of_lines = calloc(amount, sizeof(int));
				word_line = reshape_data_line(text, amount, lengths_of_lines);
				for (int i = 0; i < amount; ++i) {
					quick_sort(word_line[i], lengths_of_lines[i], sizeof(char *), compare_alphabet);
				}
				printf("Sorted succesfully.\n\n");
				break;
			}
			case 3: {
				while (1) {
				printf("1 - sort all words starting with vowels\n2 - sort in each line\n3 - do not sort\n4 - back\n");
				int vowel_choice = scan_choice(4);

				switch (vowel_choice) {
				case 1: {
					if (vowel_all)
						clean_char_array(vowel_all, total_vowel_amount);
					vowel_all = vowels_all(text, amount, &total_vowel_amount);
					quick_sort(vowel_all, total_vowel_amount, sizeof(char *), compare_alphabet);
					printf("Found and sorted succesfully.\n\n");
					break;
				}
				case 2: {
					if (vowel_line)
						clean_arrays(vowel_line, amount, lengths_of_vowels);
					lengths_of_vowels = calloc(amount, sizeof(int));
					vowel_line = vowels_line(text, amount, lengths_of_vowels);
					for (int i = 0; i < amount; ++i) {
						quick_sort(vowel_line[i], lengths_of_vowels[i], sizeof(char *), compare_alphabet);
					}
					printf("Found and sorted succesfully.\n\n");
					break;
				}
				case 3: {
					if (vowel_line)
						clean_arrays(vowel_line, amount, lengths_of_vowels);
					lengths_of_vowels = calloc(amount, sizeof(int));
					vowel_line = vowels_line(text, amount, lengths_of_vowels);
					printf("Found successfully.\n\n");
					break;
				}
				case 4: {
					break;
				}
				}
				if (vowel_choice == 4)
					break;
				}
			break;
			}
			case 4: {
			while (1) {
			printf("1 - sort all words starting with consonants\n2 - sort in each line\n3 - do not sort\n4 - back\n");
			int cons_choice = scan_choice(4);

			switch (cons_choice) {
				case 1: {
					if (conss_all)
						clean_char_array(conss_all, total_cons_amount);
					conss_all = cons_all(text, amount, &total_cons_amount);
					quick_sort(conss_all, total_cons_amount, sizeof(char *), compare_alphabet);
					printf("Found and sorted succesfully.\n\n");
					break;
				}
				case 2: {
					if (conss_line)
						clean_arrays(conss_line, amount, lengths_of_cons);
					lengths_of_cons = calloc(amount, sizeof(int));
					conss_line = cons_line(text, amount, lengths_of_cons);
					for (int i = 0; i < amount; ++i) {
						quick_sort(conss_line[i], lengths_of_cons[i], sizeof(char *), compare_alphabet);
					}
					printf("Found and sorted succesfully\n\n");
					break;
				}
				case 3: {
					if (conss_line)
						clean_arrays(conss_line, amount, lengths_of_cons);
					lengths_of_cons = calloc(amount, sizeof(int));
					conss_line = cons_line(text, amount, lengths_of_cons);
					printf("Found succesfully\n\n");
					break;
				}
				case 4: {
					break;
				}
			}
			if (cons_choice == 4)
				break;
			}
			break;
			}
			case 5: {
				break;
			}
			}
			if (work_choice == 5)
				break;
		}
		}
		break;
	}
	
	case 5: {
			if (text != NULL) {
				printf("Save original text? 1 - yes, 2 - no\n");
				int text_choice = scan_choice(2);
				switch (text_choice) {
				case 1: {
					out = open_file_out();
					fprint_char_array(out, text, amount);
					printf("Saved succesfully.\n\n");
					fclose(out);
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (word != NULL) {
				printf("Save sorted text? 1 - yes, 2 - no\n");
				int word_choice = scan_choice(2);
				switch (word_choice) {
				case 1: {
					out = open_file_out();
					fprint_char_array(out, word, total_word_amount);
					printf("Saved succesfully.\n\n");
					fclose(out);
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (vowel_all != NULL) {
				printf("Save words starting with vowels? 1 - yes, 2 - no\n");
				int vowel_choice = scan_choice(2);
				switch (vowel_choice) {
				case 1: {
					out = open_file_out();
					fprint_char_array(out, vowel_all, total_vowel_amount);
					printf("Saved succesfully.\n\n");
					fclose(out);
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (conss_all != NULL) {
				printf("Save words starting with consonants? 1 - yes, 2 - no\n");
				int cons_choice = scan_choice(2);
				switch (cons_choice) {
				case 1: {
					out = open_file_out();
					fprint_char_array(out, conss_all, total_cons_amount);
					printf("Saved succesfully.\n\n");
					fclose(out);
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (word_line != NULL) {
				printf("Save text with words sorted in each line? 1 - yes, 2 - no\n");
				int word_line_choice = scan_choice(2);
				switch (word_line_choice) {
				case 1: {
					out = open_file_out();
					fprint_arrays(out, word_line, amount, lengths_of_lines);
					printf("Saved succesfully.\n\n");
					fclose(out);
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (vowel_line != NULL) {
				printf("Save words starting with vowels found (and sorted) in each line? 1 - yes, 2 - no\n");
				int vowel_line_choice = scan_choice(2);
				switch (vowel_line_choice) {
				case 1: {
					out = open_file_out();
					fprint_arrays(out, vowel_line, amount, lengths_of_vowels);
					printf("Saved succesfully.\n\n");
					fclose(out);
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (conss_line != NULL) {
				printf("Save words starting with consonants found (and sorted) in each line? 1 - yes, 2 - no\n");
				int cons_line_choice = scan_choice(2);
				switch (cons_line_choice) {
				case 1: {
					out = open_file_out();
					fprint_arrays(out, conss_line, amount, lengths_of_cons);
					printf("Saved succesfully.\n\n");
					fclose(out);
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
		break;
		}
	case 6: {
			if (text != NULL) {
				printf("Print original text? 1 - yes, 2 - no\n");
				int text_choice = scan_choice(2);
				switch (text_choice) {
				case 1: {
					print_char_array(text, amount);
					printf("\n");
					printf("Printed succesfully.\n\n");
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (word != NULL) {
				printf("Print sorted text? 1 - yes, 2 - no\n");
				int word_choice = scan_choice(2);
				switch (word_choice) {
				case 1: {
					print_char_array(word, total_word_amount);
					printf("\n");
					printf("Printed succesfully.\n\n");
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (vowel_all != NULL) {
				printf("Print words starting with vowels? 1 - yes, 2 - no\n");
				int vowel_choice = scan_choice(2);
				switch (vowel_choice) {
				case 1: {
					print_char_array(vowel_all, total_vowel_amount);
					printf("\n");
					printf("Printed succesfully.\n\n");
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (conss_all != NULL) {
				printf("Print words starting with consonants? 1 - yes, 2 - no\n");
				int cons_choice = scan_choice(2);
				switch (cons_choice) {
				case 1: {
					print_char_array(conss_all, total_cons_amount);
					printf("\n");
					printf("Printed succesfully.\n\n");
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (word_line != NULL) {
				printf("Print text with words sorted in each line? 1 - yes, 2 - no\n");
				int word_line_choice = scan_choice(2);
				switch (word_line_choice) {
				case 1: {
					print_arrays(word_line, amount, lengths_of_lines);
					printf("\n");
					printf("Printed succesfully.\n\n");
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (vowel_line != NULL) {
				printf("Print words starting with vowels found (and sorted) in each line? 1 - yes, 2 - no\n");
				int vowel_line_choice = scan_choice(2);
				switch (vowel_line_choice) {
				case 1: {
					print_arrays(vowel_line, amount, lengths_of_vowels);
					printf("\n");
					printf("Printed succesfully.\n\n");
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
			if (conss_line != NULL) {
				printf("Print words starting with consonants found (and sorted) in each line? 1 - yes, 2 - no\n");
				int cons_line_choice = scan_choice(2);
				switch (cons_line_choice) {
				case 1: {
					print_arrays(conss_line, amount, lengths_of_cons);
					printf("\n");
					printf("Printed succesfully.\n\n");
					break;
				}
				case 2: {
					printf("Ok.\n\n");
					break;
				}
				}
			}
		break;
	}
	case 7: {
		clean_all(text, word, vowel_all, conss_all, word_line, vowel_line, conss_line, lengths_of_lines, lengths_of_vowels, lengths_of_cons, amount, total_word_amount, total_vowel_amount, total_cons_amount); 
		break;
	}
	}
	if (main_choice == 7)
		break;
}
}		




