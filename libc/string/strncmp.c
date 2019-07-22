#include <string.h>

int strncmp(const char* s1, const char* s2, size_t len)
{
    int i = 0;
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    unsigned char c1, c2;
    do {
        if (i == len) { break; }
        c1 = (unsigned char) *p1++;
        c2 = (unsigned char) *p2++;
        if (c1 == '\0') { return c1 - c2; }
        i++;
    } while (c1 == c2);
    return c1 - c2;
}
