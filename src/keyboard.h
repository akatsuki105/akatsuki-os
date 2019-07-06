#include <stdint.h>
#include <stddef.h>

#define SCAN_CODE_SET1 0x01
#define SCAN_CODE_SET2 0x02
#define SCAN_CODE_SET3 0x03

static int i = 0;
static int j = 0;

void keyboard_input_int(void);
char getscode(void);
char getchar(void);
