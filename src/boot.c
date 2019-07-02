#include "multiboot.h"

#define VRAM_TEXTMODE 0x000B8000
#define DEF_COLOR_BLACK 0x00
#define DEF_COLOR_WHITE 0x0F
#define MAX_Y 25
#define MAX_X 80
#define MAX_XY (80 * 25)

void displayChar(char c, unsigned char foreColor, unsigned char backColor, int x, int y);

void cstart(unsigned long magic, multiboot_info_t *info)
{
    char msg[] = "Starting Akatsuki OS!";
    int strlen;
    int i;

    strlen = sizeof(msg);

    for (i = 0; i < strlen; i++) {
        displayChar(msg[i], DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 0);
    }

    for (;;) ;
}

void displayChar(char c, unsigned char foreColor, unsigned char backColor, int x, int y)
{
    unsigned short *vram_TextMode;
    unsigned short color;

    vram_TextMode = (unsigned short *)VRAM_TEXTMODE;

    color = (backColor << 4) | (foreColor & 0x0F);

    vram_TextMode += x + y * MAX_X;

    *vram_TextMode = (color << 8) | c;
}
