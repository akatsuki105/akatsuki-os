# Akatsuki OS

## Abstract

CUIの自作OSです。

まだ開発中です。

7/28日現在
![sample](https://imgur.com/G4W30Px.gif, "サンプル")

## Features

このOSには以下の特徴があります。今後増えていく予定です。

- Grub2 bootloader
- PIT
- GDT
- Keyboard Interrupt
- Multiprocessing
- Memory Management(not paging)

## Usage

### Virtual Boxなどを使う場合

```
$ ./iso.sh
```

### qemuを使う場合

```
$ ./qemu.sh
```

## References

- [OSdev.org](https://wiki.osdev.org/)
- [Shadow-log](https://zeus.shadow-net.jp/)
- [30日でできるOS自作入門](https://www.amazon.co.jp/30日でできる-OS自作入門-川合-秀実/dp/4839919844)