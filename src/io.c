#include "boot.h"

#define VRAM_TEXTMODE 0x000B8000
#define MAX_Y 25
#define MAX_X 80
#define MAX_XY (80 * 25)
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

void display_char(char c, unsigned char foreColor, unsigned char backColor, int x, int y)
{
    unsigned short *vram_TextMode;
    unsigned short color;

    vram_TextMode = (unsigned short *)VRAM_TEXTMODE;

    color = (backColor << 4) | (foreColor & 0x0F);

    vram_TextMode += x + y * MAX_X;

    *vram_TextMode = (color << 8) | c;
}

void display_cursor(unsigned short pos)
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}
