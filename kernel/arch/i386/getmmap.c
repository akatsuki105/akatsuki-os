#include <kernel/getmmap.h>
#include <stdio.h>
#include <string.h>

void getmmap(multiboot_info_t *mbt)
{
    multiboot_memory_map_t *mmap = mbt->mmap_addr;
    char *type_str;

    printf("\n\n================= memory map ======================\n");

    printf("Total Memory: %dKB\n", mbt->mem_upper);

    for (mmap; mmap < (mbt->mmap_addr + mbt->mmap_length); mmap++)
    {
        if (mmap->base_addr_high == 0x0) {
            printf("base_addr = 0x%x: ", mmap->base_addr_low);
        } else {
            printf("base_addr = 0x%x%x: ", mmap->base_addr_high, mmap->base_addr_low);
        }

        if (mmap->length_high == 0x0) {
            printf("length = 0x%x: ", mmap->length_low);
        } else {
            printf("length = 0x%x%x: ", mmap->length_high, mmap->length_low);
        }
        switch (mmap->type) {
            case 0x1:
                strcpy(type_str, "available RAM");
                break;
            case 0x2:
                strcpy(type_str, "reserved RAM");
                break;
            case 0x03:
                strcpy(type_str, "ACPI reclaimable RAM");
                break;
            case 0x04:
                strcpy(type_str, "ACPI non-volatile storage RAM");
                break;
        }
        printf("type = %s\n", type_str);
    }

    printf("===================================================\n\n");
}
