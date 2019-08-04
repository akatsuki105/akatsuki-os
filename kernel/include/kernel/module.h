#ifndef _MODULE_H_
#define _MODULE_H_

const struct multiboot_mod_list *first_mod_list(const struct multiboot_info *info);
const struct multiboot_mod_list *next_mod_list(const struct multiboot_info *info, const struct multiboot_mod_list *mod);

#endif _MODULE_H_
