section .text
global inb
global outb
global io_hlt
global io_stihlt
global io_cli
global io_sti

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
