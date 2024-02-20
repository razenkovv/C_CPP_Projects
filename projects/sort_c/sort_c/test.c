#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "array.h"
#include "int_array.h"
#include "string_array.h"

int main() {
    printf("Tests:\n\n");
    printf("Int: \n");
    int in[5]  = {1, -5 , 6, 4, 5};
    int out1[5] = {1, 25, 36, 16, 25};
    int out2[4] = {1, 6, 4, 5};
    int out3[5] = {-5, 1, 4, 5, 6};
    int out4[9] = {1, 25, 36, 16, 25, 1, 6, 4, 5};

    struct ArrayN_1 *input = AddToStruct1(in, 5, sizeof(int));
    struct ArrayN_1 *output1 = map1(input, int_square);
    struct ArrayN_1 *output2 = where1(input, int_positive);
    struct ArrayN_1 *output3 = int_sort(input);
    struct ArrayN_1 *output4 = concat1(output1, output2);

    int error = 0;

    for (int i = 0; i < 5; ++i) {
	if (out1[i] != ((int *)output1->list)[i]) {
	    printf("Error in test 1\n");
	    error = 1;
	    break;
	}
    }
    for (int i = 0; i < 4; ++i) {
	if (out2[i] != ((int *)output2->list)[i]) {
	    printf("Error in test 2\n");
	    error = 1;
	    break;
	}
    }
    for (int i = 0; i < 5; ++i) {
	if (out3[i] != ((int *)output3->list)[i]) {
	    printf("Error in test 3\n");
	    error = 1;
	    break;
	}
    }
    for (int i = 0; i < 9; ++i) {
	if (out4[i] != ((int *)output4->list)[i]) {
	    printf("Error in test 4\n");
	    error = 1;
	    break;
	}
    }
    
    if (!error)
	printf("Correct.\n\n");

    printf("String:\n");
    char **word_array = malloc(3 * sizeof(char*));
    word_array[0] = malloc(3 * sizeof(char));
    word_array[1] = malloc(5 * sizeof(char));
    word_array[2] = malloc(7 * sizeof(char));
    word_array[0] = "abc\0";
    word_array[1] = "hello\0";
    word_array[2] = "goodbye\0";
    size_t *lengths = malloc(3 * sizeof(size_t));
    lengths[0] = 4;
    lengths[1] = 6;
    lengths[2] = 8;
 
    char **_out1 = malloc(3 * sizeof(char*));
    _out1[0] = "ABC"; _out1[1] = "HELLO"; _out1[2] = "GOODBYE";
    char **_out2= malloc(3 * sizeof(char*));
    _out2[0] = "ab"; _out2[1] = "hell"; _out2[2] = "goodby";
    char **_out3 = malloc(1 * sizeof(char*));
    _out3[0] = "abc";
    char **_out4= malloc(3 * sizeof(char*));
    _out4[0] = "abc"; _out4[1] = "goodbye"; _out4[2] = "hello";
    char **_out5 = malloc(4 * sizeof(char*));
    _out5[0] = "ab"; _out5[1] = "hell"; _out5[2] = "goodby", _out5[3] = "abc";

    struct ArrayN_2 *_input = AddToStruct2((void **)word_array, lengths, 3,  sizeof(char));
    struct ArrayN_2 *_output1 = map2(_input, letter_up); 
    struct ArrayN_2 *_output2 = map2(_input, remove_last_letter); 
    struct ArrayN_2 *_output3 = where2(_input, string_a_first); 
    struct ArrayN_2 *_output4 = string_sort(_input); 
    struct ArrayN_2 *_output5 = concat2(_output2, _output3); 
    
    error = 0;
    
    for (int i = 0; i < 3; ++i) {
	if (strcmp(_out1[i], ((char **)_output1->list)[i])) {
	    printf("Error in test 1\n");
	    error = 1;
	    break;
	}
    }	
    for (int i = 0; i < 3; ++i) {
	if (strcmp(_out2[i], ((char **)_output2->list)[i])) {
	    printf("Error in test 2\n");
	    error = 1;
	    break;
	}
    }	
    for (int i = 0; i < 1; ++i) {
	if (strcmp(_out3[i], ((char **)_output3->list)[i])) {
	    printf("Error in test 3\n");
	    error = 1;
	    break;
	}
    }	
    for (int i = 0; i < 3; ++i) {
	if (strcmp(_out4[i], ((char **)_output4->list)[i])) {
	    printf("Error in test 4\n");
	    error = 1;
	    break;
	}
    }	
    for (int i = 0; i < 4; ++i) {
	if (strcmp(_out5[i], ((char **)_output5->list)[i])) {
	    printf("Error in test 5\n");
	    error = 1;
	    break;
	}
    }	

    if (!error)
	printf("Correct.\n");
    return 0;
}
