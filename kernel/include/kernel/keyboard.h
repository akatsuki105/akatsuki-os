#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

//command
#define SWITCH_LED 0xED
#define SET_TYPEMATIC_RATE   0xF3     //追加
#define ENABLE_KEYBOARD      0xF4     //追加
#define SET_SCANCODESET      0xF0     //追加

#define SCAN_CODE_SET1 0x01
#define SCAN_CODE_SET2 0x02
#define SCAN_CODE_SET3 0x03

#define TYPEMATICDELAY_SET1 0x01
#define TYPEMATICDELAY_SET2 0x02
#define PORTMAP_KEYBOARD1 0x60
#define PORTMAP_KEYBOARD2 0x64

#define SET_CAPSLOCK_LED 0x04
#define SET_NUMLOCK_LED 0x02
#define SET_SCROLLLOCK_LED 0x01

#define L_SHIFT 0x2A
#define R_SHIFT 0x36
#define CAPS_LOCK 0x3A
#define NUM_LOCK 0x45
#define SCROLL_LOCK 0x46

typedef struct {
  uint8_t base[0x80];
  uint8_t shift[0x80];
  uint8_t numlock[0x80];
} keymap;

typedef struct {
  bool shift_enable : 1;
  bool relese_enable : 1;
  bool caps_lock : 1;
  bool num_lock : 1;
  bool scr_lock : 1;
  uint8_t led_stat;
} kb_stat;

void init_key(void);
uint8_t ps2_kerboard_init(void);
void keyboard_input_int(uint8_t);
uint8_t enable_keyboard(void);
uint8_t getscode(void);
uint8_t getchar(void);
uint8_t getscodeset(void);
void change_codeset(uint8_t);
void change_trate_delay(uint8_t);      //追加
void switch_kb_led(uint8_t set_led);
uint8_t scan2key(uint8_t scan_code);
