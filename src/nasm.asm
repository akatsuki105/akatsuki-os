bits 32

section .text align=4
global inb
global outb
global io_hlt
global io_stihlt
global io_cli
global io_sti
global load_gdtr
global load_idtr

inb:
    mov dx, [esp + 4]
    in al, dx
    ret

outb:
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al
    ret

io_hlt:
    hlt
    ret

io_stihlt:
    sti
    hlt
    ret

io_cli:
    cli
    ret

io_sti:
    sti
    ret

load_gdtr:		; void load_gdtr(int limit, int addr);
    mov		ax, [esp+4]		; limit
    mov		[esp+6], ax
    lgdt	[esp+6]
    ret

load_idtr:		; void load_idtr(int limit, int addr);
    mov		ax, [esp+4]		; limit
    mov		[esp+6], ax
    lidt	[esp+6]
    ret
