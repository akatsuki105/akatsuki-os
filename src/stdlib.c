void kmemset(void *start, unsigned char c, int size)
{
    unsigned char *ptr = (unsigned char *)start;
    const unsigned char ch = (const unsigned char)c;

    while (size--)
        *ptr++ = ch;
}