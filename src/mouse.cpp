#include "headers/mouse.hpp"

MouseDriver::MouseDriver(InterruptManager *manager)
    : InterruptHandler(manager, 0x2C),
      dataPort(0x60),
      commandPort(0x64)
{
    uint16_t *VideoMemory = (uint16_t *)0xb8000;
    offset = 0;
    buttons = 0;
    commandPort.Write(0xA8);
    commandPort.Write(0x20); // command 0x60 = read controller command byte
    uint8_t status = dataPort.Read() | 2;
    commandPort.Write(0x60); // command 0x60 = set controller command byte
    dataPort.Write(status);
    commandPort.Write(0xD4);
    dataPort.Write(0xF4);
    dataPort.Read();
}
MouseDriver::~MouseDriver()
{
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandPort.Read();
    if (!(status & 0x20))
        return esp;
    buffer[offset] = dataPort.Read();
    offset = (offset + 1) % 3;
    if (offset == 0)
    {
        if (buffer[1] != 0 || buffer[2] != 0)
        {
            col += buffer[1];
            if (col >= 80)
                col = 79;
            if (col < 0)
                col = 0;
            row -= buffer[2];
            if (row >= 25)
                row = 24;
            if (row < 0)
                row = 0;

            for (uint8_t i = 0; i < 3; i++)
            {
                if (((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))))
                {
                    // vga_base[row * MAX_COLS + col] = (' ' | (((0xF << 4) | 0x80) << 8));
                    // char *msg = "(00, 00), ";
                    // msg[1] = '0' + row / 10;
                    // msg[2] = '0' + row % 10;
                    // msg[5] = '0' + col / 10;
                    // msg[6] = '0' + col % 10;
                    // printf(msg);
                    // TODO: BUTTON PRESSES
                }
            }
        }
    }
    return esp;
}