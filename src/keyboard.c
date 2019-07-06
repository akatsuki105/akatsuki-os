#include "boot.h"
#include "keyboard.h"

void keyboard_input_int(void) {
    int old, scan_code, cursor_x = 0;
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

    char psend[2];

    while (1){
        scan_code = getchar();
        if (scan_code <= 0x80){
            psend[0] = keytable[scan_code];
            psend[1] = 0;
            if (i == 1) {
                if (j == 0) {
                    write_string(psend, cursor_x, 4);
                    cursor_x++;
                    old = scan_code;
                }
                else if (j > 300000) {
                    write_string(psend, cursor_x, 4);
                    cursor_x++;
                }
            }

            // 新しいキーコード
            if (old != scan_code) {
                i = 0;
                j = 0;
            } else if (old == scan_code) {
                ++j;
            }
            if (i > 300000) i = 0;
            ++i;
        }
    }
}

char getscode(void)
{
    char c = 0;
    do {
        if (inb(0x60) != c) {
            c = inb(0x60);
            if (c > 0) return c;
        }
    } while(1);
}

char getchar(void)
{
    return getscode();
}
