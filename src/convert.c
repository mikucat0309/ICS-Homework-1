#include "convert.h"

int char2int(char c) {
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 10;
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 10;
    return -1;
}

void swap(uint8_t *a, uint8_t *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}

void reverse(char *s) {
    int len = (int) strlen(s);
    for (int i = 0; i < len / 2; ++i)
        swap((uint8_t *) (&s[i]), (uint8_t *) (&s[len - 1 - i]));
}

ull n2dec(char *s, int base) {
    ull sum = 0;
    int num;
    for (int i = 0; s[i] != '\0'; ++i) {
        if ((num = char2int(s[i])) == -1)
            return -1;
        if (num >= base) // check if exceed base limit
            return -1;
        sum *= base;
        sum += num;
    }
    return sum;
}

char *dec2n(ull num, int base, int flag) {
    char *str = calloc(100 + flag, sizeof(char));
    if (!num) {
        str[0] = '0';
        return str;
    }
    int length, r;
    for (length = 0 + flag; num > 0; ++length) {
        r = num % base;
        num /= base;
        str[length] = (char) (r + '0');
        if (r > 9)
            str[length] = (char) (str[length] + 'A' - '0' - 10);
    }
    reverse(str + flag);
    if (flag)
        str[0] = '-';
    return str;
}
