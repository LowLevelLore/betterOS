#include "all.hpp"

using namespace better_os::lib;

better_os::drivers::MouseEventHandler::MouseEventHandler()
{
    x = 0;
    y = 0;
};
better_os::drivers::MouseEventHandler::~MouseEventHandler() {};
void better_os::drivers::MouseEventHandler::OnMouseDown(uint8_t button) {};
void better_os::drivers::MouseEventHandler::OnMouseUp(uint8_t button) {};
void better_os::drivers::MouseEventHandler::OnMouseMove(int32_t deltaX, int32_t deltaY) {};

better_os::drivers::VGAMouseEventHandler::VGAMouseEventHandler()
{
    x = 0;
    y = 0;
    vga_base[MAX_COLS * y + x] = ((vga_base[MAX_COLS * y + x] & 0x0F00) << 4) | ((vga_base[MAX_COLS * y + x] & 0xF000) >> 4) | ((vga_base[MAX_COLS * y + x] & 0x00FF));
};
better_os::drivers::VGAMouseEventHandler::~VGAMouseEventHandler() {};

void better_os::drivers::VGAMouseEventHandler::OnMouseDown(uint8_t button) {
    // printf("Mouse Down\t");
};
void better_os::drivers::VGAMouseEventHandler::OnMouseUp(uint8_t button) {
    // printf("Mouse Up\t");
};
void better_os::drivers::VGAMouseEventHandler::OnMouseMove(int32_t deltaX, int32_t deltaY)
{
    vga_base[MAX_COLS * y + x] = ((vga_base[MAX_COLS * y + x] & 0x0F00) << 4) | ((vga_base[MAX_COLS * y + x] & 0xF000) >> 4) | ((vga_base[MAX_COLS * y + x] & 0x00FF));
    x += (deltaX);
    if (x >= MAX_COLS)
        x = MAX_COLS - 1;
    if (x < 0)
        x = 0;
    y += (deltaY);
    if (y >= MAX_ROWS)
        y = MAX_ROWS - 1;
    if (y < 0)
        y = 0;
    vga_base[MAX_COLS * y + x] = ((vga_base[MAX_COLS * y + x] & 0x0F00) << 4) | ((vga_base[MAX_COLS * y + x] & 0xF000) >> 4) | ((vga_base[MAX_COLS * y + x] & 0x00FF));
};

better_os::drivers::MouseDriver::MouseDriver(better_os::basics::InterruptManager *manager, MouseEventHandler *handler)
    : InterruptHandler(manager, 0x2C),
      Driver("xZist/mouse"),
      handler(handler),
      dataPort(0x60),
      commandPort(0x64)
{
}

better_os::drivers::MouseDriver::~MouseDriver()
{
}

void better_os::drivers::MouseDriver::Activate()
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
};

void better_os::drivers::MouseDriver::DeActivate() {

};

int32_t better_os::drivers::MouseDriver::Reset()
{
    return 0;
};

uint32_t better_os::drivers::MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandPort.Read();
    if ((!(status & 0x20)) || !handler)
        return esp;
    buffer[offset] = dataPort.Read();
    offset = (offset + 1) % 3;
    if (offset == 0)
    {
        if (buffer[1] != 0 || buffer[2] != 0)
        {
            handler->OnMouseMove(buffer[1], -buffer[2]);
        }
        for (uint8_t i = 0; i < 3; i++)
        {
            if (((buffer[0] & (0x01 << i)) != (buttons & (0x01 << i))))
            {
                if (buttons * (0x1 << i))
                {
                    handler->OnMouseUp(i + 1);
                }
                else
                {
                    handler->OnMouseDown(i + 1);
                }
            }
        }
    }
    return esp;
}