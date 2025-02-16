
GPP = x86_64-elf-g++
AS = x86_64-elf-as
LD = x86_64-elf-ld
GPPFLAGS = -m32 -fno-pic -fno-stack-protector -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o build/gdt.o build/port.o build/interruptstubs.o build/interrupts.o build/driver.o build/keyboard.o build/mouse.o build/stdlib.o  kernel.o 

build/%.o: src/%.cpp
	@$(GPP) $(GPPFLAGS) -o $@ -c $<


%.o: %.cpp
	@$(GPP) $(GPPFLAGS) -o $@ -c $<

%.o: %.s
	@$(AS) $(ASPARAMS) -o $@ $<

build/%.o: src/%.s
	@$(AS) $(ASPARAMS) -o $@ $<

betterKernel.bin: linker.ld $(objects)
	@$(LD) $(LDPARAMS) -T $< -o $@ $(objects)

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
	@grub-mkrescue --output=betterKernel.iso iso
	@rm -rf iso *.o *.bin build/*.o
	@echo "We created an .iso file successfully, build/$@"

.PHONY: clean
clean:
	@rm -rf *.o *.bin *.iso iso/
	@rm -rf build/*

commit:
	@make clean
	@git add .
	@git commit -m "$(MSG)"
	@git push -u master


