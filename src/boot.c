#include "multiboot.h"
#include "io.h"

#define VRAM_TEXTMODE 0x000B8000
#define DEF_COLOR_BLACK 0x00
#define DEF_COLOR_WHITE 0x0F
#define MAX_Y 25
#define MAX_X 80
#define MAX_XY (80 * 25)
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT 0x3D5
#define FB_HIGH_BYTE_COMMAND 14
#define FB_LOW_BYTE_COMMAND 15
#define KBD_DATA_PORT 0x60

void display_char(char c, unsigned char foreColor, unsigned char backColor, int x, int y);
void display_cursor(unsigned short pos);
unsigned char read_keyboard(void);

void cstart(unsigned long magic, multiboot_info_t *info)
{
    int i;
    unsigned char key_code;
    char *hello_msg = "Starting Akatsuki OS!";
    char *memory_size = "Memory fuck!";

    for (i = 0; *hello_msg != 0; i++) {
        display_char(*hello_msg++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 0);
    }

    for (i = 0; *memory_size != 0; i++) {
        display_char(*memory_size++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i+8, 1);
    }

    i = 0;
    for (;;) {
        key_code = (char)(read_keyboard());
        display_char(key_code, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 2);
    }
}

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

unsigned char read_keyboard(void)
{
    return inb(KBD_DATA_PORT);
}
