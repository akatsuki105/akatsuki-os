.file "asm.s"
.text
.align  4
.globl  inb, outb, io_hlt, io_stihlt, io_cli, io_sti, load_gdtr
.type   inb, @function
.type   outb, @function
.type   io_hlt, @function
.type   io_stihlt, @function
.type   io_cli, @function
.type   io_sti, @function
.type   load_gdtr, @function

inb:
    movw 4(%esp), %dx
    in %dx, %al
    ret

outb:
    movb 8(%esp), %al # al = data
    movw 4(%esp), %dx # dx = port
    out %al, %dx # data => port
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

gdtr:
limit:  .word 0 # For limit storage
base:   .long 0 # For base storage

load_gdtr:
        movl    4(%esp),        %eax
        movl    %eax,           base
        movw    8(%esp),        %ax
        movw    %ax,            gdtr
        lgdt    gdtr
        ret
