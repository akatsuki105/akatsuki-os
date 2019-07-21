#include "kernel/keyboard.h"
#include "kernel/asm.h"
#include "kernel/fifo.h"
#include <stdio.h>

kb_stat ks;
bool ext_input;
const keymap key_code = {
    .base = {
      '0', '0', '1', '2', '3', '4', '5', '6', '7', '8',
      '9', '0', '-', '=', '\b', '\t', 'q', 'w', 'e', 'r',
      't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', '0',
      'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
      '\'', '`', '0', '\\', 'z', 'x', 'c', 'v', 'b', 'n',
      'm', ',', '.', '/', '0', '\0', '0', ' ', '0', '0',
      '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
      '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
      '\0', '\0', '0', '\0', '0', '0', '0', '0', '0', '0',
      '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
      '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
      '0', '0', '0', '0', '0', '0', '0', '0', '0', '0',
      '0', '0', '0', '0', '0', '0', '0', '0'
    },

    .shift = {
      '0', '0', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', '0', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', '0', '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', '0', '\0', '0', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '0', '\0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0'
    },

    .numlock = {
      [0x4F] = '1', [0x50] = '2', [0x51] = '3', [0x4B] = '4', [0x4C] = '5', [0x4D] = '6', [0x47] = '7', [0x48] = '8', [0x49] = '9', [0x52] = '0', [0x4A] = '-', [0x4E] = '+', [0x37] = '*', [0x53] = '.'
    }
};

void init_key(void)
{
  change_trate_delay(TYPEMATICDELAY_SET2); //追加
  if (enable_keyboard() == 0xFA) {
    printf("Keyboard enable OK\n");
  }
  if (ps2_kerboard_init() == 0) {
    printf("PS/2 Keyboard init OK\n");
  }
}

uint8_t ps2_kerboard_init(void)
{
    uint8_t scodeset = getscodeset();
    if (scodeset == 0x43) {
      printf("Current Scan code set 1\nCorrection to Scan code set2\n");
      change_codeset(SCAN_CODE_SET2);
    }
    else if (scodeset == 0x41) {
      printf("Scan code set 2\n");
    } else if (scodeset == 0x3f) {
      printf("Current Scan code set 3\nCorrection to Scan code set2\n");
      change_codeset(SCAN_CODE_SET2);
    } else {
      printf("Unknown scan code set\nPS/2 Emulation?\n");
      return 1;
    }
    return 0;
}

//下の関数を全面的に変更
void keyboard_input_int(uint8_t scan_code)
{
  uint8_t key;

  if (scan_code == 0xE0) {
    if (!ext_input) {
      ext_input = true;
    } else {
      ext_input = false;
    }
  }

  if (scan_code <= 0x80) {
    if (scan_code == L_SHIFT || scan_code == R_SHIFT) {
        ks.shift_enable = true;
      } else if (scan_code == CAPS_LOCK && !ks.caps_lock) {
        ks.caps_lock = true;
        ks.led_stat += SET_CAPSLOCK_LED;
        switch_kb_led(ks.led_stat);
      } else if (scan_code == CAPS_LOCK && ks.caps_lock) {
        ks.caps_lock = false;
        ks.led_stat -= SET_CAPSLOCK_LED;
        switch_kb_led(ks.led_stat);
      } else if (scan_code == NUM_LOCK && !ks.num_lock) {
        ks.num_lock = true;
        ks.led_stat += SET_NUMLOCK_LED;
        switch_kb_led(ks.led_stat);
      } else if (scan_code == NUM_LOCK && ks.num_lock) {
        ks.num_lock = false;
        ks.led_stat -= SET_NUMLOCK_LED;
        switch_kb_led(ks.led_stat);
      } else if (scan_code == SCROLL_LOCK && !ks.scr_lock) {
        ks.scr_lock = true;
        ks.led_stat += SET_SCROLLLOCK_LED;
        switch_kb_led(ks.led_stat);
      } else if (scan_code == SCROLL_LOCK && ks.scr_lock) {
        ks.scr_lock = false;
        ks.led_stat -= SET_SCROLLLOCK_LED;
        switch_kb_led(ks.led_stat);
      } else {
        // keymapでscancodeをkeyに変換する
        key = scan2key(scan_code);
        fifo32_put(&fifo, key);
      }
  } else {
    scan_code -= 0x80;
    if (scan_code == L_SHIFT || scan_code == R_SHIFT) {
      ks.shift_enable = false;
    }
  }
}

uint8_t enable_keyboard(void)
{
  outb(PORTMAP_KEYBOARD1, ENABLE_KEYBOARD);
  return getscode();
}

uint8_t getscodeset(void)
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

//型をuint8_tにしないとVMWareでのキー入力の挙動がおかしくなる
//ので変更
uint8_t getscode(void)
{
  uint8_t c = 0;
  do {
    if (inb(PORTMAP_KEYBOARD1) != c) {
      c = inb(PORTMAP_KEYBOARD1);
      if (c > 0)
        return c;
    }
  } while (1);
}

uint8_t getchar(void) {
  return getscode();
}

//関数名変更
void change_codeset(uint8_t set)
{
  outb(PORTMAP_KEYBOARD1, SET_SCANCODESET);
  outb(PORTMAP_KEYBOARD1, set);
}

//追加
void change_trate_delay(uint8_t set)
{
  while (inb(PORTMAP_KEYBOARD2) & 0x02);
  outb(PORTMAP_KEYBOARD1, SET_TYPEMATIC_RATE);
  while (inb(PORTMAP_KEYBOARD2) & 0x02);
  outb(PORTMAP_KEYBOARD1, set);
}

void switch_kb_led(uint8_t set_led)
{
  while (inb(PORTMAP_KEYBOARD2) & 0x02);
  outb(PORTMAP_KEYBOARD1, SWITCH_LED);
  while (inb(PORTMAP_KEYBOARD2) & 0x02);
  outb(PORTMAP_KEYBOARD1, set_led);
}

uint8_t scan2key(uint8_t scan_code)
{
  const keymap *keymap = &key_code;
  uint8_t numpad_data = 0;

  if (ks.num_lock) {
    numpad_data = keymap->numlock[scan_code];
    ext_input = false;
  }
  
  if (!numpad_data) {
    if (ext_input && scan_code == 0x35) {
      return 0;
    } else if (ks.shift_enable && !ks.caps_lock) {
      return keymap->shift[scan_code];
    } else if (!ks.shift_enable && ks.caps_lock) {
      if ((numpad_data = keymap->base[scan_code]) >= 'a' && numpad_data <= 'z'){
        return keymap->shift[scan_code];
      } else {
        return keymap->base[scan_code];
      }
    } else if (ks.shift_enable && ks.caps_lock){
      if ((numpad_data = keymap->base[scan_code]) >= 'a' && numpad_data <= 'z'){
        return keymap->base[scan_code];
      } else {
        return keymap->shift[scan_code];
      }
    } else {
      return keymap->base[scan_code];
    }
  } else {
    return numpad_data;
  }
}
