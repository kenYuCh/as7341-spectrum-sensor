
# 1 "my_helpers.c"

# 4 "/Applications/microchip/xc8/v2.40/pic/include/__size_t.h"
typedef unsigned size_t;

# 7 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdarg.h"
typedef void * va_list[1];

#pragma intrinsic(__va_start)
extern void * __va_start(void);

#pragma intrinsic(__va_arg)
extern void * __va_arg(void *, ...);

# 43 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdio.h"
struct __prbuf
{
char * ptr;
void (* func)(char);
};

# 29 "/Applications/microchip/xc8/v2.40/pic/include/c90/errno.h"
extern int errno;

# 12 "/Applications/microchip/xc8/v2.40/pic/include/c90/conio.h"
extern void init_uart(void);

extern char getch(void);
extern char getche(void);
extern void putch(char);
extern void ungetch(char);

extern __bit kbhit(void);

# 23
extern char * cgets(char *);
extern void cputs(const char *);

# 88 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdio.h"
extern int cprintf(char *, ...);
#pragma printf_check(cprintf)



extern int _doprnt(struct __prbuf *, const register char *, register va_list);


# 180
#pragma printf_check(vprintf) const
#pragma printf_check(vsprintf) const

extern char * gets(char *);
extern int puts(const char *);
extern int scanf(const char *, ...) __attribute__((unsupported("scanf() is not supported by this compiler")));
extern int sscanf(const char *, const char *, ...) __attribute__((unsupported("sscanf() is not supported by this compiler")));
extern int vprintf(const char *, va_list) __attribute__((unsupported("vprintf() is not supported by this compiler")));
extern int vsprintf(char *, const char *, va_list) __attribute__((unsupported("vsprintf() is not supported by this compiler")));
extern int vscanf(const char *, va_list ap) __attribute__((unsupported("vscanf() is not supported by this compiler")));
extern int vsscanf(const char *, const char *, va_list) __attribute__((unsupported("vsscanf() is not supported by this compiler")));

#pragma printf_check(printf) const
#pragma printf_check(sprintf) const
extern int sprintf(char *, const char *, ...);
extern int printf(const char *, ...);

# 4 "/Applications/microchip/xc8/v2.40/pic/include/c90/ctype.h"
extern __bit isalpha(char);
extern __bit isupper(char);
extern __bit islower(char);
extern __bit isdigit(char);
extern __bit isxdigit(char);
extern __bit isspace(char);
extern __bit ispunct(char);
extern __bit isalnum(char);
extern __bit isprint(char);
extern __bit isgraph(char);
extern __bit iscntrl(char);


extern char toupper(char);
extern char tolower(char);

# 13 "/Applications/microchip/xc8/v2.40/pic/include/c90/stdint.h"
typedef signed char int8_t;

# 20
typedef signed int int16_t;

# 28
typedef __int24 int24_t;

# 36
typedef signed long int int32_t;

# 52
typedef unsigned char uint8_t;

# 58
typedef unsigned int uint16_t;

# 65
typedef __uint24 uint24_t;

# 72
typedef unsigned long int uint32_t;

# 88
typedef signed char int_least8_t;

# 96
typedef signed int int_least16_t;

# 109
typedef __int24 int_least24_t;

# 118
typedef signed long int int_least32_t;

# 136
typedef unsigned char uint_least8_t;

# 143
typedef unsigned int uint_least16_t;

# 154
typedef __uint24 uint_least24_t;

# 162
typedef unsigned long int uint_least32_t;

# 181
typedef signed char int_fast8_t;

# 188
typedef signed int int_fast16_t;

# 200
typedef __int24 int_fast24_t;

# 208
typedef signed long int int_fast32_t;

# 224
typedef unsigned char uint_fast8_t;

# 230
typedef unsigned int uint_fast16_t;

# 240
typedef __uint24 uint_fast24_t;

# 247
typedef unsigned long int uint_fast32_t;

# 268
typedef int32_t intmax_t;

# 282
typedef uint32_t uintmax_t;

# 289
typedef int16_t intptr_t;




typedef uint16_t uintptr_t;

# 6 "/Applications/microchip/xc8/v2.40/pic/include/c90/stddef.h"
typedef int ptrdiff_t;

# 18 "my_helpers.h"
void dump_buffer(const void *buffer, size_t size);

# 7 "my_helpers.c"
void dump_buffer(const void *buffer, size_t size) {
const uint8_t *p = (const uint8_t *)buffer;
size_t i, j;

for (i = 0; i < size; i += 16) {
printf("%08x:", (unsigned int)i);
for (j = 0; j < 16; j++) {
if (i + j < size) {
printf(" %02x", p[i + j]);
} else {
printf("   ");
}
}
printf("  ");
for (j = 0; j < 16; j++) {
if (i + j < size) {
printf("%c", isprint(p[i + j]) ? p[i + j] : '.');
}
}
printf("\r\n");
}
}
