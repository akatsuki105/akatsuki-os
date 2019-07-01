#define ASM_FILE 1
#include "multiboot.h"
#define EXT_C(func) func
#define STACK_SIZE 0x4000
#define MULTIBOOT_HEADER_FLAGS 0x00000003

.text
.global start, _start

start:
_start:
        jmp     multiboot_entry
        .align  4

multiboot_header:
        .long   MULTIBOOT_HEADER_MAGIC
        .long   MULTIBOOT_HEADER_FLAGS
        .long   -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

multiboot_entry:
        movl    $(stack + STACK_SIZE), %esp
        pushl   $0
        popf
        pushl   %ebx
        pushl   %eax

        call    EXT_C(cstart)

loop:   hlt
        jmp     loop

        .comm   stack, STACK_SIZE