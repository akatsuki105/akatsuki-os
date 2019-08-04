#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
mkdir -p isodir/modules

cp sysroot/boot/akatsuki.kernel isodir/boot/akatsuki.kernel
cp app/sample1.c isodir/modules/sample1.c
cp app/sample2.c isodir/modules/sample2.c
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "akatsuki" {
	multiboot /boot/akatsuki.kernel
	insmod iso9660
	module /modules/sample1.c
	module /modules/sample2.c
}
EOF
grub-mkrescue -o akatsuki.iso isodir
