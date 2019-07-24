.file "asm.s"
.text
.align  4
.globl  inb, outb, io_hlt, io_stihlt, io_cli, io_sti, load_gdtr, load_idtr, as_timer_interrupt, as_keyboard_interrupt, as_software_interrupt, load_eflags, store_eflags
.type   inb, @function
.type   outb, @function
.type   io_hlt, @function
.type   io_stihlt, @function
.type   io_cli, @function
.type   io_sti, @function
.type   load_gdtr, @function
.type   load_idtr, @function
.type   as_timer_interrupt, @function
.type   as_keyboard_interrupt, @function
.type   load_eflags, @function
.type   store_eflags, @function
.extern keyboard_interrupt, timer_interrupt, software_interrupt

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

load_eflags:
    pushfl
    pop     %eax
    ret

store_eflags:
    mov     4(%esp), %eax
    push    %eax
    popfl
    ret

load_gdtr:
	movl	4(%esp), %eax
	lgdt	(%eax)
	
	mov 	$0x10, %ax
	mov 	%ax,   %ds
	mov 	%ax,   %es
	mov 	%ax,   %fs
	mov 	%ax,   %gs
	mov 	%ax,   %ss
	
	jmp     $0x08, $gdt_flush
gdt_flush:	
	ret

load_idtr:
	mov     4(%esp), %eax
	lidt    (%eax)
	ret

as_timer_interrupt:
    push    %es
    push    %ds
    pushal
    mov     %esp, %eax
    push    %eax
    mov     %ss, %ax
    mov     %ax, %ds
    mov     %ax, %es
    call    timer_interrupt
    pop     %eax
    popal
    pop     %ds
    pop     %es
    sti
    iretl

as_keyboard_interrupt:
    push    %es
    push    %ds
    pushal
    mov     %esp, %eax
    push    %eax
    mov     %ss, %ax
    mov     %ax, %ds
    mov     %ax, %es
    call    keyboard_interrupt
    pop     %eax
    popal
    pop     %ds
    pop     %es
    sti
    iretl


as_software_interrupt:
    pushl %esp
    pushl %ebp
    pushl %esi
    pushl %edi
    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax
    call software_interrupt	
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx
    popl %edi
    popl %esi
    popl %ebp
    popl %esp
    iretl