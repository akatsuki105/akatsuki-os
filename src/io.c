#include "boot.h"

#define VRAM_TEXTMODE 0x000B8000
#define MAX_Y 25
#define MAX_X 80
#define MAX_XY (80 * 25)
#define COLOR_BLACK 0x00
#define COLOR_LIGHTGREEN 0x0a
#define COLOR_WHITE 0x0f
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15

void write_string(char *target_string, int pos_x, int pos_y)
{
    int i = 0;
    for (i = 0; target_string[i] != 0; i++) {
        display_char(target_string[i], pos_x, pos_y);
        pos_x++;
        if (pos_x > 80) {
            pos_x = 0;
            pos_y++;
        }
    }
}

void display_char(char c, int x, int y)
{
    unsigned short *vram_TextMode = (unsigned short *)VRAM_TEXTMODE;;
    unsigned short color = (COLOR_BLACK << 4) | (COLOR_LIGHTGREEN & 0x0F);

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
