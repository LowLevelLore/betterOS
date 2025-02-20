#include "all.hpp"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors() {
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

using namespace better_os::lib;
extern "C" void kernelMain(const void *multiboot_structure, uint32_t /*multiboot_magic*/) {
    better_os::basics::GlobalDescriptorTable gdt;
    better_os::basics::InterruptManager interrupts(0x20, &gdt);

    printf("GDT and InterruptManager initialized.\n");
    printf("Initializing drivers.\n");

    better_os::drivers::DriverManager driverManager;

    better_os::drivers::VGAMouseEventHandler mouseHandler;
    better_os::drivers::MouseDriver mouse(&interrupts, &mouseHandler);
    driverManager.AddDriver(&mouse);
    better_os::drivers::VGAKeyboardEventHandler keyboardHandler;
    better_os::drivers::KeyboardDriver keyboard(&interrupts, &keyboardHandler);
    driverManager.AddDriver(&keyboard);

    better_os::hardware::PeripheralComponentInterconnectController pciController;
    pciController.SelectDrivers(&driverManager, &interrupts);

    printf("All Drivers Initialized.\n");

    interrupts.Activate();

    printf("Interrupts Activated.\n\n");
    printf("betterOS - boot successful\n\t\t\t\tBy: xZist\n===============================================================================\n\n");

    while (1);
}