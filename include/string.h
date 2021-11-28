#ifndef STRING_H
#define STRING_H

#include <types.h>

static inline void *memsetl(void *s, unsigned long c, size_t count)
{
	unsigned long *tmp = (unsigned long *)s;

	while (count--)
		*tmp++ = c;
	return s;
}

static inline void *memsetw(void *s, unsigned short c, size_t count)
{
	unsigned short *tmp = (unsigned short *)s;

	while (count--)
		*tmp++ = c;
	return s;
}

static inline void *memset(void *s, int c, size_t count)
{
	unsigned char *tmp = (unsigned char *)s;

	while (count--)
		*tmp++ = c;
	return s;
}

static inline void bzero(void *s, size_t count)
{
	unsigned char *tmp = (unsigned char *)s;

	while (count--)
		*tmp++ = 0;
}

static inline void *memcpy(void *to, const void *from, size_t n)
{
	unsigned char *tmp = (unsigned char *)to, *s = (unsigned char *)from;

	while (n--)
		*tmp++ = *s++;
	return to;
}

static inline int memcmp(const void *cs, const void *ct, size_t count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for (su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2))
			break;
	return res;
}

static inline void *memchr(const void *s, int c, size_t n)
{
	const unsigned char *p = s;

	while (n--)
		if ((unsigned char)c == *p++)
			return (void *)(p-1);
	return NULL;
}

static inline char *strchr(const char *s, int c)
{
	for ( ; *s != (char)c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *)s;
}

static inline size_t strlen(const char *str)
{
	const char *s;

	if (str == 0)
		return(0);
	for (s = str; *s; ++s);

	return s - str;
}

static inline char *strrchr(const char *s, int c)
{
	const char *p = s + strlen(s);
	do {
		if (*p == (char)c)
			return (char *)p;
	} while( --p >= s );

	return NULL;
}

static inline char *strcpy(char *dest, const char *org)
{
	char *temp = dest;

	for (; (*dest = *org); ++dest, ++org);

	return temp;
}

static inline char *strncpy(char *dest, const char *org, size_t n)
{
	char *d = dest;
	const char *s = org;

	if (n) {
		do {
			if ((*d++ = *s++) == 0) {
				while (--n)
					*d++ = 0;
				break;
			}
		} while (--n != 0);
	}
	return dest;
}

static inline size_t strnlen(const char *s, unsigned int count)
{
	const char *sc;

	if (!s)
		return 0;
	for ( sc = s; ( count-- ) && ( *sc != '\0' ); ++sc );

	return sc - s;
}

static inline char *strcat(char *s, const char *append)
{
	char *save = s;

	for (; *s; ++s);
	while ((*s++ = *append++));

	return save;
}

static inline char *strncat(char *dest, const char *src, size_t count)
{
	char *tmp = dest;

	if (count) {
		while (*dest)
			dest++;
		while ((*dest++ = *src++)) {
			if (--count == 0) {
				*dest = '\0';
				break;
			}
		}
	}

	return tmp;
}

static inline int strcmp(const char *s1, const char *s2)
{
	while ((*s2 != '\0') && (*s2 == *s1)) {
		s1++;
		s2++;
	}
	return *s1 - *s2;
}

static inline int strncmp(const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return 0;
	do
	{
		if (*s1 != *s2++)
			return *(unsigned const char *)s1 - *(unsigned const char *)--s2;
		if (*s1++ == 0)
			break;
	} while (--n != 0);

	return 0;
}

int strcasecmp(const char *s1, const char *s2);
int strncasecmp(const char *s1, const char *s2, size_t n);
char *strstr(const char *s, const char *find);
char *strpbrk(const char *cs, const char *ct);
char *strsep(char **s, const char *ct);

#endif
