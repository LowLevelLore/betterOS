
GPPFLAGS = -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o kernel.o

kernel.o: kernel.cpp
	@g++ $(GPPFLAGS) -m32 -o kernel.o -c kernel.cpp

loader.o: loader.s
	@as $(ASPARAMS) -o loader.o loader.s

betterKernel.bin: linker.ld $(objects)
	@ld $(LDPARAMS) -T $< -o $@ $(objects)

clean:
	@rm -rf *.o *.bin *.iso iso/

betterKernel.iso: betterKernel.bin
	@mkdir -p build/
	@mkdir -p iso/
	@mkdir -p iso/boot/
	@mkdir -p iso/boot/grub/
	@cp betterKernel.bin iso/boot/
	@> iso/boot/grub/grub.cfg
	@echo 'set timeout=0' >> iso/boot/grub/grub.cfg
	@echo 'set default=0' >> iso/boot/grub/grub.cfg
	@echo '' >> iso/boot/grub/grub.cfg
	@echo 'menuentry "Better OS" {' >> iso/boot/grub/grub.cfg
	@echo ' multiboot /boot/betterKernel.bin' >> iso/boot/grub/grub.cfg
	@echo ' boot' >> iso/boot/grub/grub.cfg
	@echo '}' >> iso/boot/grub/grub.cfg

	@grub-mkrescue --output=build/$@ iso
	@rm -rf iso *.o *.bin
