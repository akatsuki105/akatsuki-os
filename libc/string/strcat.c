#include <stdio.h>

char *strcat(char *s1, const char *s2)
{
    int i, j;
    for (i = 0; s1[i] != 0; i++);

    for (j = 0; s2[j] != 0; j++) {
        s1[i++] = s2[j];
    }
    s1[i] = 0;
    return s1;
}