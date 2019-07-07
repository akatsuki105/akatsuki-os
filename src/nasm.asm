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
global asm_keyboard_interrupt

extern keyboard_interrupt

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

asm_keyboard_interrupt:
    push	es
    push	ds
    pushad
    mov		eax,esp
    push	eax
    mov		ax,ss
    mov		ds,ax
    mov		es,ax
    call	keyboard_interrupt
    pop		eax
    popad
    pop		ds
    pop		es
    sti
    iretd