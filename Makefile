GPP = x86_64-elf-g++
AS = x86_64-elf-as
LD = x86_64-elf-ld
GPPFLAGS = -m32 -fno-pic -fno-stack-protector -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Iinclude
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = build/loader.o build/basics/gdt.o build/hardware/pci.o build/hardware/port.o build/basics/interruptstubs.o \
			build/basics/interrupts.o build/drivers/driver.o build/drivers/vga.o build/drivers/keyboard.o \
			build/drivers/mouse.o build/lib/stdlib.o  build/gui/widget.o build/gui/desktop.o build/gui/window.o build/kernel.o 

build/%.o: src/%.cpp structure
	@$(GPP) $(GPPFLAGS) -o $@ -c $< 

build/%.o: src/%.s structure
	@$(AS) $(ASPARAMS) -o $@ $<

betterKernel.bin: linker.ld $(objects) structure
	@$(LD) $(LDPARAMS) -T $< -o $@ $(objects)

betterKernel.iso: betterKernel.bin structure
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
	@grub-mkrescue --output=build/betterKernel.iso iso
	@rm -rf iso *.o *.bin build/*.o
	@echo "We created an .iso file successfully, build/$@"

.PHONY: clean
clean:
	@rm -rf *.o *.bin *.iso iso/
	@rm -rf build

commit:
	@make clean
	@git add .
	@git commit -m "$(MSG)"
	@git push -u master

structure:
	@mkdir -p build
	@mkdir -p build/basics
	@mkdir -p build/hardware
	@mkdir -p build/drivers
	@mkdir -p build/lib
	@mkdir -p build/gui

