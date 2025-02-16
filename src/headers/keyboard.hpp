#ifndef __BETTER_OS_KEYBOARD_H
#define __BETTER_OS_KEYBOARD_H

#include "types.hpp"
#include "interrupts.hpp"
#include "port.hpp"

static const char keymap[128] = {
    0, 0, '1', '2', '3', '4', '5', '6',       // 0x00-0x07
    '7', '8', '9', '0', '-', '=', '\b', '\t', // 0x08-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',   // 0x10-0x17
    'o', 'p', '[', ']', '\n', 0, 'a', 's',    // 0x18-0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   // 0x20-0x27
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',   // 0x28-0x2F
    'b', 'n', 'm', ',', '.', '/', 0, '*',     // 0x30-0x37
    0, ' ', 0, 0, 0, 0, 0, 0,                 // 0x38-0x3F
};

static const char shiftedKeymap[128] = {
    0, 0, '!', '@', '#', '$', '%', '^',       // 0x00-0x07
    '&', '*', '(', ')', '_', '+', '\b', '\t', // 0x08-0x0F
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',   // 0x10-0x17
    'O', 'P', '{', '}', '\n', 0, 'A', 'S',    // 0x18-0x1F
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',   // 0x20-0x27
    '"', '~', 0, '|', 'Z', 'X', 'C', 'V',     // 0x28-0x2F
    'B', 'N', 'M', '<', '>', '?', 0, '*',     // 0x30-0x37
    0, ' ', 0, 0, 0, 0, 0, 0,                 // 0x38-0x3F
};

class KeyboardDriver : public InterruptHandler
{
    bool capsOn = false;
    bool shiftPressed = false;

    Port8Bit dataPort;
    Port8Bit commandPort;

public:
    KeyboardDriver(InterruptManager *manager);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif // !__BETTER_OS_KEYBOARD_H