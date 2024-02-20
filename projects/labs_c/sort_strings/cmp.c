#include <string.h>


int compare_alphabet(const void *x1, const void *x2) {
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

int compare_int(int x1, int x2) {
    return x1-x2;
}