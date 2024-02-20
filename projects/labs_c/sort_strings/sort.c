#include <stddef.h>

void swap(void *b, void *e, size_t size) {
	char *t1 = b;
	char *t2 = e;
	do {
		char tmp = *t1;
		*t1++ = *t2;
		*t2++ = tmp;
	} while (--size > 0);
}


void bubble_sort(void *base, size_t n, size_t size, int (*cmp) (const void *, const void *)) {
	if (n <= 1)
		return;
		
	char *b = (char *) base;
	char *e = b + (n - 1) * size;
	char *run = b;
	int swap_check = 0;
	for (int i = 0; i < (n - 1); ++i) { 
		for (run = b; run < (e - size * i); run += size) {
			if ((*cmp) ((void *) (run + size), (void *) run) < 0) {
				swap((void *) run, (void *) (run + size), size);
				swap_check = 1;
			}
		}
		if (swap_check == 0)
			break;
		swap_check = 0;
	}
}


void insert_sort(void *base, size_t n, size_t size, int (*cmp) (const void *, const void *)) {
	if (n <= 1)
		return;
	char *b = (char *) base;
	char *tmp = b;
	char *run = tmp + size;
	char *e = b + (n - 1) * size;
    
	while (run <= e) {
		if ((*cmp) ((void *) run, (void *) tmp) < 0)
			tmp = run;
		run += size;
	}
	if (tmp != b)
		swap((void *) tmp, (void *) b, size);

	run = b + size * 2;
	while (run <= e) {
		tmp = run; 
		while (((*cmp) ((void *) tmp, (void *) (tmp - size)) < 0)) {
			swap((void *) (tmp - size), (void *) tmp, size);
			tmp -= size;
		}
		run += size;
	}
}


void quick_sort(void *base, size_t n, size_t size, int (*cmp) (const void *, const void *)) {
	if (n <= 4) {
		if (n <= 1)
			return;
		else 
			insert_sort(base, n, size, cmp); 
		return;
	}
	char *b = (char *) base;
	char *e = b + (n - 1) * size;
	char *pivot = b + (n / 2) * size;	
	
	do {
		while ((*cmp)((void *) b, (void *) pivot) < 0)
			b += size;
		while ((*cmp)((void *) e, (void *) pivot) > 0)
			e -= size;
		if (b < e) {
			swap((void *) b,(void *) e, size);
			if (pivot == b)
				pivot = e;
			else if (pivot == e)
				pivot = b;
			b += size;
			e -= size;
		 } else if (e == b) {
			 b += size;
			 e -= size;
			 break;
		 }
	} while (b <= e);
	
	int len = (e - (char *)base + size)/size;
	void *start = (void *) ((char *) base + len * size);
	quick_sort(base, len, size, cmp);
	quick_sort(start, n - len, size, cmp);
}
