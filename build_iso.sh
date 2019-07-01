mkdir -p boot/grub
cp grub.cfg boot/grub
cp src/akatsuki.elf boot/
grub-mkrescue -o os.iso boot/