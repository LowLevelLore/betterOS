#include "headers/keyboard.hpp"

void printf(const char *str);

KeyboardDriver::KeyboardDriver(InterruptManager *interruptManager) : InterruptHandler(0x21, interruptManager), dataPort(0x60), commandPort(0x64)
{
    while (commandPort.Read() & 0x1)
    {
        dataPort.Read();
    }
    commandPort.Write(0xAE); // Tells PIC to start sending interrupts
    commandPort.Write(0x20); // Get Current status
    uint8_t status = (dataPort.Read() | 1) & -0x10;
    commandPort.Write(0x60);
    dataPort.Write(status);
    dataPort.Write(0xF4);
}
KeyboardDriver::~KeyboardDriver() {}
uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataPort.Read();
    char msg[] = "KEYBOARD 0x00";
    char hex[] = "0123456789ABCDEF";
    msg[11] = hex[(key >> 4) & 0x0F];
    msg[12] = hex[(key) & 0x0F];
    printf(msg);
    return esp;
};