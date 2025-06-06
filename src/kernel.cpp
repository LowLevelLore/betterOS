#include "all.hpp"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void callConstructors() {
    for (constructor* ctor = &start_ctors; ctor != &end_ctors; ++ctor) {
        (*ctor)();
    }
}

#define GRAPHICS_MODE

using namespace better_os::lib;

void taskA() {
    while (true)
        printf("A");
}
void taskB() {
    while (true)
        printf("B");
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/) {
    // Initialize global descriptor table and interrupt manager.
    better_os::basics::GlobalDescriptorTable globalDescriptorTable;
    better_os::basics::TaskManager taskManager;
    better_os::basics::Task task1(&globalDescriptorTable, taskA);
    better_os::basics::Task task2(&globalDescriptorTable, taskB);
    // taskManager.AddTask(&task1);
    // taskManager.AddTask(&task2);

    uint32_t* memupper = (uint32_t*)(((size_t)((uint64_t)(multiboot_structure))) + 8);
    size_t heap = 10 * 1024 * 1024;
    better_os::basics::MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    better_os::basics::InterruptManager interruptManager(0x20, &globalDescriptorTable, &taskManager);

    printf("GDT and InterruptManager initialized.\n");
    printf("Initializing drivers.\n");

    // Set up driver manager.
    better_os::drivers::DriverManager driverManager;

    // Initialize and register mouse driver.
    better_os::drivers::VGAMouseEventHandler vgaMouseEventHandler;
    better_os::drivers::MouseDriver mouseDriver(&interruptManager, &vgaMouseEventHandler);
    driverManager.AddDriver(&mouseDriver);

    // Initialize and register keyboard driver.
    better_os::drivers::VGAKeyboardEventHandler vgaKeyboardEventHandler;
    better_os::drivers::KeyboardDriver keyboardDriver(&interruptManager, &vgaKeyboardEventHandler);
    driverManager.AddDriver(&keyboardDriver);

    // Initialize PCI controller and select drivers.
    better_os::hardware::PeripheralComponentInterconnectController pciController;
    pciController.SelectDrivers(&driverManager, &interruptManager);
    printf("All Drivers Initialized.\n");

    // Set up VGA graphics.
    // better_os::drivers::VideoGraphicsArray_320x200x8 vgaHandler;

    better_os::drivers::amd_am79c973* eth0 = driverManager.netDriver;

    // Activate interrupts.
    interruptManager.Activate();
    printf("Interrupts Activated.\n\n");
    printf("betterOS - boot successful\n\t\t\t\tBy: xZist\n===============================================================================\n\n");

    // Clear the text screen area.
    for (int i = 0; i < 100; i++) {
        printf("\n");
    }

    eth0->Send((uint8_t*)"Hello Network", 13);

    printf("heap: 0x");
    printhex((heap >> 24) & 0xFF);
    printhex((heap >> 16) & 0xFF);
    printhex((heap >> 8) & 0xFF);
    printhex((heap) & 0xFF);

    void* allocated = memoryManager.malloc(1024);
    printf("\nallocated: 0x");
    printhex(((uint64_t)allocated >> 24) & 0xFF);
    printhex(((uint64_t)allocated >> 16) & 0xFF);
    printhex(((uint64_t)allocated >> 8) & 0xFF);
    printhex(((uint64_t)allocated) & 0xFF);
    printf("\n");

    // Set the graphics mode and draw a rectangle.
    // vgaHandler.SetMode();
    // vgaHandler.FillRectangle(0, 0, 320, 200, 0x00, 0x00, 0xA8);

    while (1);
}
