#!/usr/bin/bash

make clean

make betterKernel.iso
qemu-system-x86_64 \
    -machine pc \
    -cdrom build/betterKernel.iso \
    -boot d \
    -device pcnet,netdev=net0 \
    -netdev user,id=net0 \
    -vga std \
    -serial stdio \
    -drive id=disk,file=disk.img,format=raw,if=ide &
vncviewer :5900
