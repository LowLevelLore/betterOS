#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::drivers;

MouseEventHandler::MouseEventHandler() {
    m_x = 0;
    m_y = 0;
}

MouseEventHandler::~MouseEventHandler() {}

void MouseEventHandler::OnMouseDown(uint8_t button) {}

void MouseEventHandler::OnMouseUp(uint8_t button) {}

void MouseEventHandler::OnMouseMove(int32_t deltaX, int32_t deltaY) {}

VGAMouseEventHandler::VGAMouseEventHandler() {
    m_x = 0;
    m_y = 0;
    vga_base[MAX_COLS * m_y + m_x] = ((vga_base[MAX_COLS * m_y + m_x] & 0x0F00) << 4) |
                                     ((vga_base[MAX_COLS * m_y + m_x] & 0xF000) >> 4) |
                                     (vga_base[MAX_COLS * m_y + m_x] & 0x00FF);
}

VGAMouseEventHandler::~VGAMouseEventHandler() {}

void VGAMouseEventHandler::OnMouseDown(uint8_t button) {
    // printf("Mouse Down\t");
}

void VGAMouseEventHandler::OnMouseUp(uint8_t button) {
    // printf("Mouse Up\t");
}

void VGAMouseEventHandler::OnMouseMove(int32_t deltaX, int32_t deltaY) {
    vga_base[MAX_COLS * m_y + m_x] = ((vga_base[MAX_COLS * m_y + m_x] & 0x0F00) << 4) |
                                     ((vga_base[MAX_COLS * m_y + m_x] & 0xF000) >> 4) |
                                     (vga_base[MAX_COLS * m_y + m_x] & 0x00FF);
    m_x += deltaX;
    if (m_x >= MAX_COLS)
        m_x = MAX_COLS - 1;
    if (m_x < 0)
        m_x = 0;
    m_y += deltaY;
    if (m_y >= MAX_ROWS)
        m_y = MAX_ROWS - 1;
    if (m_y < 0)
        m_y = 0;
    vga_base[MAX_COLS * m_y + m_x] = ((vga_base[MAX_COLS * m_y + m_x] & 0x0F00) << 4) |
                                     ((vga_base[MAX_COLS * m_y + m_x] & 0xF000) >> 4) |
                                     (vga_base[MAX_COLS * m_y + m_x] & 0x00FF);
}

MouseDriver::MouseDriver(better_os::basics::InterruptManager *manager, MouseEventHandler *handler)
    : InterruptHandler(manager, 0x2C),
      Driver("xZist/mouse"),
      m_handler(handler),
      m_dataPort(0x60),
      m_commandPort(0x64) {}

MouseDriver::~MouseDriver() {}

void MouseDriver::Activate() {
    // VideoMemory is not used further; kept for compatibility.
    uint16_t *VideoMemory = (uint16_t *)0xb8000;
    m_offset = 0;
    m_buttons = 0;
    m_commandPort.Write(0xA8);
    m_commandPort.Write(0x20);  // command 0x20 = read controller command byte
    uint8_t status = m_dataPort.Read() | 2;
    m_commandPort.Write(0x60);  // command 0x60 = set controller command byte
    m_dataPort.Write(status);
    m_commandPort.Write(0xD4);
    m_dataPort.Write(0xF4);
    m_dataPort.Read();
}

void MouseDriver::Deactivate() {
}

int32_t MouseDriver::Reset() {
    return 0;
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
    uint8_t status = m_commandPort.Read();
    if ((!(status & 0x20)) || !m_handler)
        return esp;
    m_buffer[m_offset] = m_dataPort.Read();
    m_offset = (m_offset + 1) % 3;
    if (m_offset == 0) {
        if (m_buffer[1] != 0 || m_buffer[2] != 0) {
            m_handler->OnMouseMove(m_buffer[1], -m_buffer[2]);
        }
        for (uint8_t i = 0; i < 3; i++) {
            if ((m_buffer[0] & (0x01 << i)) != (m_buttons & (0x01 << i))) {
                if (m_buttons * (0x1 << i)) {
                    m_handler->OnMouseUp(i + 1);
                } else {
                    m_handler->OnMouseDown(i + 1);
                }
            }
        }
    }
    return esp;
}
