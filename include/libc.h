#ifndef GAME_LIBC_H
#define GAME_LIBC_H

/*
    Functions
*/

int LIBC_sprintf(char * str, char * format, ...);
int LIBC_rand();
void LIBC_initrand(unsigned int seed);
int LIBC_printf(char * format, ...);
char * LIBC_strcpy(char * dst, char * src);
void LIBC_bzero(unsigned char * p, int n);
char * LIBC_memcpy(char * dst, char * src, int size);
char * memcpy(char * dst, char * src, int size);
void LIBC_puts(char * str);
char * LIBC_memchr(char * str, char c, int size);
char * LIBC_memmove(char * dst, char * src, int size);
char * LIBC_strcat(char * dst, char * src);

#endif