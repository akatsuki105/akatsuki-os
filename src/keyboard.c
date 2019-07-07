#include "boot.h"
#include <stdint.h>
#include <stddef.h>

static int i = 0;
static int j = 0;
struct FIFO32 fifo;
int buf[30];

#define SET_TYPEMATIC_RATE 0xF3
#define ENABLE_KEYBOARD 0xF4   
#define SET_SCANCODESET 0xF0
#define SCAN_CODE_SET1 0x01
#define SCAN_CODE_SET2 0x02
#define SCAN_CODE_SET3 0x03
#define TYPEMATICDELAY_SET1 0x01
#define TYPEMATICDELAY_SET2 0x02
#define PORTMAP_KEYBOARD1 0x60
#define PORTMAP_KEYBOARD2 0x64

void init_keyboard (void)
{
    change_trate_delay(TYPEMATICDELAY_SET2);
    if (enable_keyboard() == 0xFA) {
        write_string("Keyboard enable OK", 0, 3);
    } else {
        write_string("fuck", 0, 3);
    }
    if (ps2_kerboard_init() == 0) {
        write_string("PS/2 Keyboard init OK", 0, 3);
    }
    write_string("ps2_kerboard_init end", 0, 3);
}

char ps2_kerboard_init(void)
{
    change_codeset(SCAN_CODE_SET2);
    char scodeset = getscodeset();
    if (scodeset == 0x43) {
        write_string("Scan code set 1", 0, 3);
    } else if (scodeset == 0x41) {
        write_string("Scan code set 2", 0, 3);
    } else if (scodeset == 0x3f) {
        write_string("Scan code set 3", 0, 3);
    } else {
        write_string("Unknown scan code set", 0, 3);
        return 1;
    }
    outb(0x60, 0xFA);
    return 0;
}

void keyboard_input_int(char scan_code)
{
    char keytable[0x80] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x08, 0,
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '@', '[', 0x0a, 0, 'A', 'S',
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', ':', 0, 0, ']', 'Z', 'X', 'C', 'V',
        'B', 'N', 'M', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
        '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0x5c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x5c, 0, 0
    };

    if (scan_code < 0x80) {
        fifo32_put(&fifo, keytable[scan_code]);
    }
}

char enable_keyboard(void)
{
    outb(PORTMAP_KEYBOARD1, ENABLE_KEYBOARD);
    return getscode();
}

char getscodeset(void)
{
    while (inb(PORTMAP_KEYBOARD2) & 0x02);
    outb(PORTMAP_KEYBOARD1, SET_SCANCODESET);
    if (getscode() == 0xFA) {
        while (inb(PORTMAP_KEYBOARD2) & 0x02);
        outb(PORTMAP_KEYBOARD1, 0x00);
        return getscode();
    } else {
        return 0x00;
    }
}

char getscode(void){
    char c = 0;
    do {
        if (inb(PORTMAP_KEYBOARD1) != c) {
        c = inb(PORTMAP_KEYBOARD1);
        if (c > 0) return c;
        }
    } while (1);
}

char getchar(void)
{
    return getscode();
}

void change_codeset(char set)
{
    outb(PORTMAP_KEYBOARD1, SET_SCANCODESET);
    outb(PORTMAP_KEYBOARD1, set);
}

void change_trate_delay(char set)
{
    while (inb(PORTMAP_KEYBOARD2) & 0x02);
    outb(PORTMAP_KEYBOARD1, SET_TYPEMATIC_RATE);
    while (inb(PORTMAP_KEYBOARD2) & 0x02);
    outb(PORTMAP_KEYBOARD1, set);   
}
