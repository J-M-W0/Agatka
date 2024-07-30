#ifndef __STRING_H
#define __STRING_H

#include <stdbool.h>

int str_len(const char * ptr);
int str_nlen(const char * ptr, int max);
bool isdigit(char c);
int to_numeric_digit(char c);

#endif // __STRING_H
