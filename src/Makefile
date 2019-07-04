TARGET = akatsuki.elf

CC = gcc
LN_S = ln -s
RANLIB = ranlib
INCLUDES = -I./include
FLAGS = -m32 -ffreestanding -fno-common -fno-builtin -fomit-frame-pointer -O2 -c
LD = ld  -melf_i386  -Ttext=0x100000 --oformat elf32-i386 -o

.S.o:
	${CC} ${INCLUDES} ${FLAGS} $<
.s.o:
	${CC} ${FLAGS} $<
.c.o:
	${CC} ${INCLUDES} ${FLAGS} $<

BOOT_S = load.S
BOOT_ASM = io.s
BOOT_C = boot.c stdlib.c

BOOT_OBJ=${BOOT_S:.S=.o} ${BOOT_ASM:.s=.o} ${BOOT_C:.c=.o}

all:    akatsuki

akatsuki:   ${BOOT_OBJ}
	${LD} ${TARGET} ${BOOT_OBJ}

${BOOT_OBJ}:    ${BOOT_SRC}
clean::
	-${RM}  -f *~ *.lo *.o