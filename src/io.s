.file "io.s"
.text
.align  4
.globl  inb, outb, io_hlt, io_stihlt, io_cli, io_sti
.type   inb, @function
.type   outb, @function
.type   io_hlt, @function
.type   io_stihlt, @function
.type   io_cli, @function
.type   io_sti, @function

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
