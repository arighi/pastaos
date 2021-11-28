#include <ctype.h>
#include <string.h>

int strcasecmp(const char *s1, const char *s2)
{
	register const unsigned char
			*us1 = (const unsigned char *)s1,
			*us2 = (const unsigned char *)s2;

	while (tolower(*us1) == tolower(*us2++))
		if (*us1++ == '\0')
			return 0;

	return tolower(*us1) - tolower(*--us2);
}

int strncasecmp(const char *s1, const char *s2, size_t n)
{
	if (n) {
		register const unsigned char
				*us1 = (const unsigned char *)s1,
				*us2 = (const unsigned char *)s2;
		do
		{
			if (tolower(*us1) != tolower(*us2++))
				return tolower(*us1) - tolower(*--us2);
			if (*us1++ == '\0')
				break;
		} while (--n != 0);
	}
	return 0;
}

char *strstr(const char *s, const char *find)
{
	char c, sc;
	size_t len;

	if ((c = *find++) != 0) {
		len = strlen(find);
		do {
			do {
				if ((sc = *s++) == 0)
					return 0;
			} while(sc != c);
		} while (strncmp(s, find, len) != 0);
		s--;
	}
	return (char *)s;
}

char *strpbrk(const char *cs, const char *ct)
{
	const char *sc1, *sc2;

	for (sc1 = cs; *sc1 != '\0'; ++sc1) {
		for (sc2 = ct; *sc2 != '\0'; ++sc2) {
			if (*sc1 == *sc2)
				return (char *)sc1;
		}
	}
	return NULL;
}

char *strsep(char **s, const char *ct)
{
	char *sbegin = *s, *end;

	if (sbegin == NULL)
		return NULL;

	end = strpbrk(sbegin, ct);
	if (end)
		*end++ = '\0';
	*s = end;

	return sbegin;
}
