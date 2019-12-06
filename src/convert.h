#ifndef MYFIRSTC_CONVERT_H
#define MYFIRSTC_CONVERT_H
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <tchar.h>

#define ull unsigned long long

ull n2dec(char *s, int base);

char *dec2n(ull num, int base, int flag);