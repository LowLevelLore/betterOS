#include "src/headers/types.hpp"
#include "src/headers/interrupts.hpp"
#include "src/headers/gdt.hpp"
#include "src/headers/keyboard.hpp"
#include "src/headers/mouse.hpp"
#include "src/headers/stdlib.hpp"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
  for (constructor *i = &start_ctors; i != &end_ctors; i++)
    (*i)();
}

extern "C" void kernelMain(const void *multiboot_structure, uint32_t /*multiboot_magic*/)
{
  printf("betterOS - boot successful\n\t\tBy: xZist\n\n");

  GlobalDescriptorTable gdt;
  InterruptManager interrupts(0x20, &gdt);

  MouseDriver mouse(&interrupts);
  KeyboardDriver keyboard(&interrupts);

  interrupts.Activate();

  while (1)
    ;
}