#include <stdio.h>
#include "mystring.h"
#include <stdlib.h>
#include <time.h>

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

void search(const char *s, int *lMax, int *kMax) {
	char *k;
	const char *s2 = s;
	while (*s) {
		const char *s1 = s;
		k = strchr(s, ('1'));
		if (k != NULL) {
			int len = 0, i = 0;
			s1 = k;
			len = strspn(s1, ("1"));
			s1 += len;
			if (len > *lMax) {
				*lMax = len;
				*kMax = k - s2;
			}
			s = s1;
			len = 0;
		}
		else break;
	}
}

char *rmSpaces(const char *s) {
	int i = 0, l = 0;
	char *res = calloc(strlen(s)+1, sizeof(char));
	char *r = res;
	l = strspn(s, (" 	"));
	s += l;
	while (*s) {
		const char *s1 = s;
		l = strcspn(s1, (" 	"));
		s1 += l;
		memcpy(r, s, s1 - s);
		r += s1 - s;
		l = strspn(s1, (" 	"));
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

char *work(const char *s) {
	int lMax = 0, kMax = 0;
 	int length;
	length = strlen(s);
		
	search(s, &lMax, &kMax); 
	char *s2 = NULL;
	s2 = (char*)calloc(length - lMax + 1, sizeof(char));
	s2[length - lMax] = ('\0');
	s2 = strncpy(s2, s, kMax);
	strncpy(s2 + kMax, s + kMax + lMax, length - kMax - lMax);
	char *s3 = NULL;
			
	s3 = rmSpaces(s2);
	free(s2);
	return s3;
}

int main() {
	printf ("1 - work with my text\n2 - check time\n");
	int choice, ch;
	ch = scanf ("%d", &choice);
	scanf("%*c");
	if ((ch <= 0) || (choice != 1) && (choice != 2)) 
		printf ("You have entered something incorrect\n");
	else if (choice == 1) {
		printf ("Enter your text:\n");
			char *s = NULL;
			do {
				s = get_str();
				if (s) {
					char *s3 = NULL;
					s3 = work(s);
					printf("Input : \"%s\"\n", s);
					printf("Output: \"%s\"\n", s3);
					free(s);
					free(s3);
				}	
			} while (s);
		}

		else {
			printf ("Enter string length and number of lines. "); 
			printf ("Program will create these lines and measure time.\n");
			printf ("length: ");
			int length;
			scanf ("%d", &length);
			printf ("Number: ");
			int number;
			scanf ("%d", &number);
			char set[] = {'0', '1', ' '};
			double time_total = 0;
			clock_t t_all = clock();
			for (int i = 0; i < number; ++i) {
				char *data = calloc(length + 1, sizeof(char));
				int select;
				srand(time(NULL) % (i+1000000));
				for (int j = 0; j < length; ++j) {
					select = (rand()%3);
					data[j] = set[select];
				}
				data[length] = ('\0');
				char *s3 = NULL;
				clock_t t = clock();
				s3 = work(data);
				free(s3);
				t = clock() - t;
				double time = (double)t / CLOCKS_PER_SEC;
				free(data);
				time_total += time;
				if ((i % 500) == 0)
					printf ("%d\n", i);
			}
			t_all = clock() - t_all;
			double tAll = (double)t_all / CLOCKS_PER_SEC;
			double avgtime;
			avgtime = time_total / number;
			printf ("For %d lines, each %d length, average time is %.8lf seconds, total time is %.8lf seconds.\n",
			number, length, avgtime, time_total);
			printf ("Total program time is %.8lf seconds.\n", tAll);
		}
return 0;
}
