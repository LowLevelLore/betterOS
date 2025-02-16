#ifndef __BETTER_OS_KEYBOARD_H
#define __BETTER_OS_KEYBOARD_H

#include "types.hpp"
#include "interrupts.hpp"
#include "port.hpp"
#include "driver.hpp"

class KeyboardEventHandler
{
protected:
    static constexpr char keymap[128] = {
        0, 0, '1', '2', '3', '4', '5', '6',       // 0x00-0x07
        '7', '8', '9', '0', '-', '=', '\b', '\t', // 0x08-0x0F
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',   // 0x10-0x17
        'o', 'p', '[', ']', '\n', 0, 'a', 's',    // 0x18-0x1F
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',   // 0x20-0x27
        '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',   // 0x28-0x2F
        'b', 'n', 'm', ',', '.', '/', 0, '*',     // 0x30-0x37
        0, ' ', 0, 0, 0, 0, 0, 0,                 // 0x38-0x3F
    };

    static constexpr char shiftedKeymap[128] = {
        0, 0, '!', '@', '#', '$', '%', '^',       // 0x00-0x07
        '&', '*', '(', ')', '_', '+', '\b', '\t', // 0x08-0x0F
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',   // 0x10-0x17
        'O', 'P', '{', '}', '\n', 0, 'A', 'S',    // 0x18-0x1F
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',   // 0x20-0x27
        '"', '~', 0, '|', 'Z', 'X', 'C', 'V',     // 0x28-0x2F
        'B', 'N', 'M', '<', '>', '?', 0, '*',     // 0x30-0x37
        0, ' ', 0, 0, 0, 0, 0, 0,                 // 0x38-0x3F
    };
    bool capsOn = false;
    bool shiftPressed = false;

public:
    KeyboardEventHandler();
    ~KeyboardEventHandler();

    virtual void onKeyPress(uint8_t scanCode);
    virtual void onKeyRelease(uint8_t scanCode);
};

class VGAKeyboardEventHandler : public KeyboardEventHandler
{
public:
    VGAKeyboardEventHandler();
    ~VGAKeyboardEventHandler();

    void onKeyPress(uint8_t scanCode);
    void onKeyRelease(uint8_t scanCode);
};

class KeyboardDriver : public InterruptHandler, public Driver
{
    Port8Bit dataPort;
    Port8Bit commandPort;
    KeyboardEventHandler *handler;

public:
    KeyboardDriver(InterruptManager *manager, KeyboardEventHandler *handler);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
    virtual int32_t Reset();
    virtual void DeActivate();
};

#endif // !__BETTER_OS_KEYBOARD_H