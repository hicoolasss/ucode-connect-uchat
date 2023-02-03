#include "../inc/libmx.h"
void *mx_memchr(const void *s, int c, size_t n) {
	char *b = (char *)s;
	for (;n > 0; n--, b++)
		if (*b == (char)c)
			return b;
	return NULL;
}
