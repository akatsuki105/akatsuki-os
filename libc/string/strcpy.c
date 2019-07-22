#include <string.h>

char *strcpy(char *s1, const char *s2)
{
    int i;
    for (i = 0; s2[i] != 0; i++) {
        s1[i] = s2[i];
    }
    return s1;
}
