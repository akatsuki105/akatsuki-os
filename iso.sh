#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/akatsuki.kernel isodir/boot/akatsuki.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "akatsuki" {
	multiboot /boot/akatsuki.kernel
}
EOF
grub-mkrescue -o akatsuki.iso isodir
