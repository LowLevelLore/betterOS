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

void sysprintf(char* str) {
    asm("int $0x80" : : "a"(4), "b"(str));
}

void taskA() {
    while (true)
        sysprintf("A");
}
void taskB() {
    while (true)
        sysprintf("B");
}

extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/) {
    // Initialize global descriptor table and interrupt manager.
    better_os::basics::GlobalDescriptorTable globalDescriptorTable;
    better_os::basics::TaskManager taskManager;
    better_os::basics::Task task1(&globalDescriptorTable, taskA);
    better_os::basics::Task task2(&globalDescriptorTable, taskB);
    taskManager.AddTask(&task1);
    taskManager.AddTask(&task2);

    uint32_t* memupper = (uint32_t*)(((size_t)((uint64_t)(multiboot_structure))) + 8);
    size_t heap = 10 * 1024 * 1024;
    better_os::basics::MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    better_os::basics::InterruptManager interruptManager(0x20, &globalDescriptorTable, &taskManager);
    better_os::SyscallHandler syscalls(&interruptManager, 0x80);

    printf("GDT and InterruptManager initialized.\n");
    printf("Initializing drivers.\n");

    better_os::drivers::DriverManager driverManager;

    better_os::drivers::VGAMouseEventHandler vgaMouseEventHandler;
    better_os::drivers::MouseDriver mouseDriver(&interruptManager, &vgaMouseEventHandler);
    driverManager.AddDriver(&mouseDriver);

    better_os::drivers::VGAKeyboardEventHandler vgaKeyboardEventHandler;
    better_os::drivers::KeyboardDriver keyboardDriver(&interruptManager, &vgaKeyboardEventHandler);
    driverManager.AddDriver(&keyboardDriver);

    better_os::hardware::PeripheralComponentInterconnectController pciController;
    pciController.SelectDrivers(&driverManager, &interruptManager);
    printf("All Drivers Initialized.\n");

    better_os::drivers::amd_am79c973* eth0 = driverManager.netDriver;

    better_os::drivers::AdvancedTechnologyAttachment ata0m(true, 0x1F0);
    void* data = ata0m.Identify();
    if (data) {
        delete[] data;
    }

    interruptManager.Activate();
    printf("Interrupts Activated.\n\n");
    printf("betterOS - boot successful\n\t\t\t\tBy: xZist\n===============================================================================\n\n");

    for (int i = 0; i < 100; i++) {
        printf("\n");
    }

    eth0->Send((uint8_t*)"Hello Network", 13);

    const char* website = "http://www.AlgorithMan.de";
    uint8_t dataToWrite[512] = {0};
    for (size_t i = 0; i < 25; ++i)
        dataToWrite[i] = website[i];

    ata0m.Write28(0, dataToWrite, 512);
    ata0m.Flush();

    char* readData = ata0m.Read28(0, 64);
    if (readData) {
        printf(readData);
        delete[] readData;
    }

    while (1);
}
