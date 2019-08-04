#include <kernel/multiboot.h>
#include <stddef.h>

const struct multiboot_mod_list *first_mod_list(const struct multiboot_info *info)
{
    if (!(info->flags != MULTIBOOT_INFO_MODS)) {
        return NULL;
    }
    if (!info->mods_count) {
        return NULL;
    }
    return (const struct multiboot_mod_list *)info->mods_addr;
}

const struct multiboot_mod_list *next_mod_list(const struct multiboot_info *info, const struct multiboot_mod_list *mod)
{
    const struct multiboot_mod_list *first = (const struct multiboot_mod_list *)info->mods_addr;
    ++mod;

    if (mod - first >= info->mods_count) {
        return NULL;
    }
    return mod;
}