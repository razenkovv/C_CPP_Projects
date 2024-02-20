#include <stddef.h>

size_t strlen(const char *s) {
	const char *tmp = s;
	while (*tmp != ('\0'))
		++tmp;
	return (tmp - s);
}

char *strncpy(void *dst, const void *src, size_t len) {
	int lensrc;
	lensrc = strlen(src);
	int l;
	l = len;
	if (lensrc < len)
		len = lensrc;

	int i = 0, m;
	unsigned long *longdst = dst;
	const unsigned long *longsrc = src;
	for (i, m = len/sizeof(long); i < m; ++i)
		*(longdst++) = *(longsrc++);
	
	unsigned char *chardst = (unsigned char*)longdst;
	const unsigned char *charsrc = (unsigned char*)longsrc;
	i = 0;
	for (i, m = len % sizeof(long); i < m; ++i)
		*(chardst++) = *(charsrc++);
	
	while (len < l) {
		*(chardst++) = ('\0');
		++len;
	}
	return ((char*)dst);
}



void *memcpy(void *dst, const void *src, size_t len) {
	int i = 0, m;
	unsigned long *longdst = dst;
	const unsigned long *longsrc = src;
	for (i, m = len/sizeof(long); i < m; ++i)
		*(longdst++) = *(longsrc++);
	
	unsigned char *chardst = (unsigned char*)longdst;
	const unsigned char *charsrc = (unsigned char*)longsrc;
	i = 0;
	for (i, m = len % sizeof(long); i < m; ++i)
		*(chardst++) = *(charsrc++);
return dst;
}

char *strchr(const char *s, int sym) {
	while (*s != (char)sym) {
		++s;
		if (!(*s))
			return NULL;
	}
	return (char*)s;
}


char *strncpy_2(char *dst, const char *src, size_t len) {
	int lensrc;
	lensrc = strlen(src);
	int l;
	l = len;
	if (lensrc < len)
		len = lensrc;
	char *tmp = dst;
	while ((tmp - dst) < len)
		*(tmp++) = *(src++);

		while (len < l) {
			*(tmp++) = ('\0');
			++len;
		}
		return (dst);
}


void *memcpy_2(void *dst, const void *src, size_t len) {
	unsigned char *chardst = (unsigned char*)dst;
	const unsigned char *charsrc = (unsigned char*)src;
	int i = 0;
	while ((chardst - (unsigned char*)dst) < len)
		*(chardst++) = *(charsrc++);
return dst;
}

size_t strspn(const char *dst, const char *src) {
	int flag = 0;
	size_t len = 0;
	while (*dst != ('\0')) {
		const char *tmp = src;
		while (*tmp != ('\0')) {
			if (*tmp == *dst){
				flag = 1;
				++len;
				break;
			}
			++tmp;
		}
		if (flag == 0)
			break;
		flag = 0;
		++dst;
	}
	return len;
}

size_t strcspn(const char *dst, const char *src) {
	int flag = 0;
	size_t len = 0;
	while (*dst != ('\0')) {
		const char *tmp = src;
		while (*tmp != ('\0')) {
			if (*tmp == *dst){
				flag = 1;
				break;
			}
			++tmp;
		}
		if (flag == 1)
			break;
		flag = 0;
		++len;
		++dst;
	}
	return len;
}
