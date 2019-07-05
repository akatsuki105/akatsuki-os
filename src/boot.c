#include "multiboot.h"
#include "boot.h"

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

char keytable[0x80] = {
    0,   0,   '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08, 0,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0x0a, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0,   0,   ']', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', ',', '.', '/', 0,   '*', 0,   ' ', 0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0x5c, 0,  0,   0,   0,   0,   0,   0,   0,   0,   0x5c, 0,  0
};

void cstart(unsigned long magic, multiboot_info_t *info)
{
    int i, x, y;
    char *s;
    unsigned char scan_code, ascii_code;
    // hello msg
    char *hello_msg = "Starting Akatsuki OS!";
    // memory msg
    char *memory_msg = "Memory size: ";
    int memory_size = (int)info->mem_upper;

    for (i = 0; *hello_msg != 0; i++) {
        display_char(*hello_msg++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 0);
    }

    for (i = 0; *memory_msg != 0; i++) {
        display_char(*memory_msg++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 1);
    }
    sprintf(s, "%d", memory_size);
    for (; *s != 0; i++) {
        display_char(*s++, DEF_COLOR_WHITE, DEF_COLOR_BLACK, i, 1);
    }

    x = 0;
    y = 2;
    for (;;) {
        scan_code = (read_keyboard());
        ascii_code = keytable[scan_code];
        scan_code = 0;
        display_char(ascii_code, DEF_COLOR_WHITE, DEF_COLOR_BLACK, x, y);
        if (ascii_code != 0) {
            x++;
            if (x == 80) {
                x = 0;
                y++;
            }
        }
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
