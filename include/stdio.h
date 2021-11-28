#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

#define STR_MAX_LENGTH	1024
#define EOF		-1

int getchar(void);
int putchar(int c);

int printf(const char *__restrict __format, ...);
int puts(const char *__s);

int snprintf(char *__restrict __s, size_t __maxlen, const char *__restrict __format, ...);
int vsnprintf(char *__restrict __s, size_t __maxlen, const char *__restrict __format, va_list __arg);

int sprintf(char *buf, const char *fmt, ...);

char *getline(char *__s, int __size);
char *gets(char *__s);

int scanf(const char *__restrict __format, ...);
int sscanf(const char *__restrict __buf, const char *__restrict __fmt, ...);
int vsscanf(const char *__restrict __s, const char *__restrict __format, va_list __arg);

#endif /* STDIO_H */
