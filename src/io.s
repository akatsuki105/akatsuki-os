.file "io.s"
.text
.align  4
.globl  inb, outb
.type   inb, @function
.type   outb, @function

inb:
    movw 4(%esp), %dx
    in %dx, %al
    ret

outb:
    movb 8(%esp), %al # al = data
    movw 4(%esp), %dx # dx = port
    out %al, %dx # data => port
    ret
