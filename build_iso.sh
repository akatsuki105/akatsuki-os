mkdir -p isofiles/boot/grub
cp grub.cfg isofiles/boot/grub
cp src/akatsuki.elf isofiles/boot/
grub-mkrescue -o os.iso isofiles/