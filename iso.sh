#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
mkdir -p isodir/modules

cp sysroot/boot/akatsuki.kernel isodir/boot/akatsuki.kernel
cp app/program isodir/modules/program
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "akatsuki" {
	multiboot /boot/akatsuki.kernel
	module /modules/program
}
EOF
grub-mkrescue -o akatsuki.iso isodir
