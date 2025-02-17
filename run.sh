make clean

make betterKernel.iso
qemu-system-i386 -cdrom build/betterKernel.iso &
vncviewer
