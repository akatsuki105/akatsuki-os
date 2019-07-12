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
#define TYPEMATICDELAY_SET1 0x01
#define PORTMAP_KEYBOARD1 0x60
#define PORTMAP_KEYBOARD2 0x64
#define KEYSTA_SEND_NOTREADY	0x02

void wait_KBC_sendready(void)
{
	/* �L�[�{�[�h�R���g���[�����f�[�^���M�\�ɂȂ�̂�҂� */
	for (;;) {
		if ((inb(PORTMAP_KEYBOARD2) & KEYSTA_SEND_NOTREADY) == 0) {
			break;
		}
	}
	return;
}

void init_keyboard(void)
{
	wait_KBC_sendready();
	outb(PORTMAP_KEYBOARD2, 0x60);
	wait_KBC_sendready();
	outb(PORTMAP_KEYBOARD1, 0x47);
	return;
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
