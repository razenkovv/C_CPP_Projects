#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "array.h"
#include "int_array.h"
#include "string_array.h"

int main() {
    printf("1 - integer; 2 - string\n");
    int q = 0;
    scanf("%d", &q);
    if (q == 1) {
	size_t dimension = 0;
	int *a = int_read_input(&dimension);
	struct ArrayN_1 *input = AddToStruct1(a, dimension, sizeof(int));
	struct ArrayN_1 *output1 = map1(input, int_square);
	struct ArrayN_1 *output2 = where1(input, int_positive);
	struct ArrayN_1 *output3 = int_sort(input);
	struct ArrayN_1 *output4 = concat1(output1, output2);
	printf("Input: ");
	int_print_output(input);
	printf("\nOutput(map): ");
	int_print_output(output1);
	printf("\nOutput(where): ");
	int_print_output(output2);
	printf("\nOutput(sort): ");
	int_print_output(output3);
	printf("\nOutput(concat): ");
	int_print_output(output4);
	int_free_struct(input);
	int_free_struct(output1);
	int_free_struct(output2);
	int_free_struct(output3);
	int_free_struct(output4);
    
    } else {
	int amount = 0, total_word_amount = 0;
	char **text_array = NULL;
	text_array = string_get_data(&amount);
	char **word_array = NULL;
	word_array = string_reshape_data_all(text_array, amount, &total_word_amount);
	for (int i = 0; i < amount; ++i) {
	    free(text_array[i]);
	}
	free(text_array);
	size_t *lengths = malloc(total_word_amount * sizeof(size_t));
	for (int i = 0; i < total_word_amount; ++i) {
	    lengths[i] = strlen(word_array[i]) + 1;
	}
	struct ArrayN_2 *input = AddToStruct2((void **)word_array, lengths, total_word_amount, sizeof(char));
	struct ArrayN_2 *output1 = map2(input, letter_up); 
	struct ArrayN_2 *output2 = map2(input, remove_last_letter); 
	struct ArrayN_2 *output3 = where2(input, string_a_first); 
	struct ArrayN_2 *output4 = string_sort(input); 
	struct ArrayN_2 *output5 = concat2(output2, output3); 
	printf("Input:\n");
        string_print_output(input);
	printf("\nOutput(map 1):\n");
	string_print_output(output1);
	printf("\nOutput(map 2):\n");
	string_print_output(output2);
	printf("\nOutput(where):\n");
	string_print_output(output3);
	printf("\nOutput(sort):\n");
	string_print_output(output4);
	printf("\nOutput(concat):\n");
	string_print_output(output5);
	string_free_struct(input);
	string_free_struct(output1);
	string_free_struct(output2);
	string_free_struct(output3);
	string_free_struct(output4);
	string_free_struct(output5);
    }

	return 0;
}
