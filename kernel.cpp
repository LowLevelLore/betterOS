#include "src/headers/types.hpp"
#include "src/headers/interrupts.hpp"
#include "src/headers/gdt.hpp"
#include "src/headers/keyboard.hpp"
#include "src/headers/mouse.hpp"
#include "src/headers/stdlib.hpp"
#include "src/headers/driver.hpp"

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

  GlobalDescriptorTable gdt;
  InterruptManager interrupts(0x20, &gdt);

  printf("GDT and InterruptManager initialized.\n");
  printf("Initializing drivers.\n");

  DriverManager driverManager;

  VGAMouseEventHandler mouseHandler;
  MouseDriver mouse(&interrupts, &mouseHandler);
  driverManager.AddDriver(&mouse);
  VGAKeyboardEventHandler keyboardHandler;
  KeyboardDriver keyboard(&interrupts, &keyboardHandler);
  driverManager.AddDriver(&keyboard);

  printf("All Drivers Initialized.\n");

  interrupts.Activate();

  printf("Interrupts Activated.\n\n");
  printf("betterOS - boot successful\n\t\t\t\tBy: xZist\n===============================================================================\n\n");

  while (1)
    ;
}