.file "io.s"
.text
.align  4
.globl  outb
.type   outb, @function

outb:
    movb 8(%esp), %al
    movw 4(%esp), %dx
    out %al, %dx
    ret
