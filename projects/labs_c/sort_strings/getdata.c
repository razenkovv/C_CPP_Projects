#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define BUF 5

char *get_str() {
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

char **get_data(int *amount) {
	char **data = NULL;
	char *res = NULL;
	int  i = 0, j = 0;
	data = (char **) malloc(BUF * sizeof(char *));
	do {
		res = get_str();
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

char *fget_str(FILE *in) {
	char buf[81] = {0};
	char *res = NULL;
	int len = 0;
	int n = 0;
	do {
		n = fscanf(in, "%80[^\n]", buf);
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
			fscanf(in, "%*c");
		}
	} while (n > 0);

	if (len > 0) {
		res[len] = '\0';
	} else {
		res = (char*)calloc(1, sizeof(char));
	}
	return res;
}

char **fget_data(FILE *in, int *amount) {
	char **data = NULL;
	char *res = NULL;
	int i = 0, j = 0;
	data = (char **) malloc(BUF * sizeof(char *));
	do {
		res = fget_str(in);
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

int scan_choice(int n) {
	while (1) {
		int choice = 0, check = 0;
		int error = 1;
		char buf[2] = {0}; 
		check = scanf("%2[^\n]", buf);
		if ((check) && (strlen(buf) == 1)) {
			choice = atoi(buf);
			for (int i = 1; i <= n; ++i) {
				if (choice == i) {
					error = 0;
					break;
				}	
			}
		}
	
		scanf("%*[^\n]");
		scanf("%*c");
		if (error == 0) {
			return choice;
			break;
		} else
			printf("Please reenter: ");
	}
}

FILE *open_file_in() {
	printf("Enter filename: ");
	char *name = calloc(82, sizeof(char));
	int check = 0;
Again:
	check = scanf("%81[^\n]", name);
	if (check == 0) {
		printf("Can't read. Please reenter: ");
		scanf("%*c");
		goto Again;
	}
	scanf("%*[^\n]");
	scanf("%*c");
	int len = strlen(name);
	if (strlen(name) == 81) {
		printf("Too long name. Please reenter: ");
		goto Again;
	}
	if (name[len - 1] != 't') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 1] != 't') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 2] != 'x') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 3] != 't') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 4] != '.') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	
	FILE *in;
	in = fopen(name, "r");
	if (errno) {
		free(name);
		name = calloc(82, sizeof(char));
		perror(NULL);
		printf("Please reenter: ");
		errno = 0;
		goto Again;
	}
	if (name)
		free(name);
	return in;
}

FILE *open_file_out() {
	printf("Enter filename: ");
	char *name = calloc(82, sizeof(char));
	int check = 0;
Again:
	check = scanf("%81[^\n]", name);
	if (check == 0) {
		printf("Can't read. Please reenter: ");
		scanf("%*c");
		goto Again;
	}
	scanf("%*[^\n]");
	scanf("%*c");
	int len = strlen(name);
	if (strlen(name) == 81) {
		printf("Too long name. Please reenter: ");
		goto Again;
	}
	if (name[len - 1] != 't') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 1] != 't') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 2] != 'x') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 3] != 't') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	if (name[len - 4] != '.') {
		printf("Please enter .txt file: ");
		goto Again;
	}
	FILE *out;
	out = fopen(name, "w");
	if (errno) {
		free(name);
		name = calloc(82, sizeof(char));
		perror(NULL);
		printf("Please reenter: ");
		errno = 0;
		goto Again;
	}
	if (name)
		free(name);
	return out;
}
