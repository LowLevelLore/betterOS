
GPPFLAGS = -m32 -fno-stack-protector -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
LDPARAMS = -melf_i386

objects = loader.o build/gdt.o kernel.o

build/%.o: src/%.cpp
	@g++ $(GPPFLAGS) -o $@ -c $<


%.o: %.cpp
	@g++ $(GPPFLAGS) -o $@ -c $<

%.o: %.s
	@as $(ASPARAMS) -o $@ $<

betterKernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)

clean:
	@rm -rf *.o *.bin *.iso iso/
	@rm -rd build/*

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
	@echo "We created an .iso file successfully, build/$@"

