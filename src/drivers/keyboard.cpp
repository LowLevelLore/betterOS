#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::drivers;

KeyboardEventHandler::KeyboardEventHandler() {
    shiftPressed = false;
    capsOn = false;
}

KeyboardEventHandler::~KeyboardEventHandler() {}

void KeyboardEventHandler::onKeyPress(uint8_t scanCode) {
    // Handle modifier state changes
    switch (scanCode) {
        case 0x2A:  // Left Shift
        case 0x36:  // Right Shift
            shiftPressed = true;
            break;
        case 0x3A:  // Caps Lock
            capsOn = !capsOn;
            break;
    }
}

void KeyboardEventHandler::onKeyRelease(uint8_t scanCode) {
    switch (scanCode) {
        case 0x2A:  // Left Shift release
        case 0x36:  // Right Shift release
            shiftPressed = false;
            break;
    }
}

VGAKeyboardEventHandler::VGAKeyboardEventHandler() {}
VGAKeyboardEventHandler::~VGAKeyboardEventHandler() {}

void VGAKeyboardEventHandler::onKeyPress(uint8_t scanCode) {
    KeyboardEventHandler::onKeyPress(scanCode);

    bool isExtended = scanCode & 0x80;
    uint8_t baseCode = scanCode & 0x7F;

    // Handle special keys first
    switch (baseCode) {
        case 0x1C:
            if (shiftPressed) {
                clear_screen();
                break;
            }
            putchar('\n');
            return;  // Enter
        case 0x0E:
            putchar('\b');
            return;  // Backspace
        case 0x39:
            putchar(' ');
            return;  // Space
    }

    if (baseCode < sizeof(keymap)) {
        char ch = shiftPressed ? shiftedKeymap[baseCode] : keymap[baseCode];
        if (isExtended) {
            switch (baseCode) {
                case 0x48:
                    ch = '^';
                    break;  // Up
                case 0x50:
                    ch = 'v';
                    break;  // Down
                case 0x4B:
                    ch = '<';
                    break;  // Left
                case 0x4D:
                    ch = '>';
                    break;  // Right
            }
        }

        // Caps Lock logic
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            bool effectiveShift = shiftPressed ^ capsOn;
            ch = effectiveShift ? shiftedKeymap[baseCode] : keymap[baseCode];
        }

        if (ch != 0) {
            putchar(ch);
        }
    }
}

void VGAKeyboardEventHandler::onKeyRelease(uint8_t scanCode) {
    KeyboardEventHandler::onKeyRelease(scanCode);
}

KeyboardDriver::KeyboardDriver(better_os::basics::InterruptManager* manager, KeyboardEventHandler* handler)
    : InterruptHandler(manager, 0x21),
      Driver("xZist/keyboard"),
      m_handler(handler),
      m_dataPort(0x60),
      m_commandPort(0x64) {}

KeyboardDriver::~KeyboardDriver() {}

void KeyboardDriver::Activate() {
    while (m_commandPort.Read() & 0x1)
        m_dataPort.Read();
    m_commandPort.Write(0xAE);  // activate interrupts
    m_commandPort.Write(0x20);  // command 0x20 = read controller command byte
    uint8_t status = (m_dataPort.Read() | 1) & ~0x10;
    m_commandPort.Write(0x60);  // command 0x60 = set controller command byte
    m_dataPort.Write(status);
    m_dataPort.Write(0xF4);
}

void KeyboardDriver::DeActivate() {
}

int32_t KeyboardDriver::Reset() {
    return 0;
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    static bool extendedKey = false;
    uint8_t key = m_dataPort.Read();

    if (key == 0xE0) {
        extendedKey = true;
        return esp;
    }

    bool isKeyUp = (key & 0x80) || (key == 0xAA) || (key == 0xB6);
    uint8_t makeCode = key & 0x7F;

    if (extendedKey) {
        // Handle extended key logic
        extendedKey = false;
        if (!isKeyUp) {
            m_handler->onKeyPress(makeCode | 0x80);  // Mark as extended
        }
        return esp;
    }

    if (isKeyUp) {
        m_handler->onKeyRelease(makeCode);
    } else {
        m_handler->onKeyPress(makeCode);
    }

    return esp;
}
