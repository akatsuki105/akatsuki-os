#include <string.h>

int strcmp(const char* s1, const char* s2)
{
    const unsigned char *p1 = (const unsigned char *) s1;
    const unsigned char *p2 = (const unsigned char *) s2;
    unsigned char c1, c2;
    do {
        c1 = (unsigned char) *p1++;
        c2 = (unsigned char) *p2++;
        if (c1 == '\0') { return c1 - c2; }
    } while (c1 == c2);
    return c1 - c2;
}
