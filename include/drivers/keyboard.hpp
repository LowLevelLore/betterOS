#ifndef __BETTER_OS_KEYBOARD_H
#define __BETTER_OS_KEYBOARD_H

#include "../basics/interrupts.hpp"
#include "../hardware/port.hpp"
#include "../lib/types.hpp"
#include "driver.hpp"

namespace better_os {
namespace drivers {
class KeyboardEventHandler {
   protected:
    static constexpr char keymap[128] = {
        0, 0, '1', '2', '3', '4', '5', '6',        // 0x00-0x07
        '7', '8', '9', '0', '-', '=', '\b', '\t',  // 0x08-0x0F
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',    // 0x10-0x17
        'o', 'p', '[', ']', '\n', 0, 'a', 's',     // 0x18-0x1F
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',    // 0x20-0x27
        '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',    // 0x28-0x2F
        'b', 'n', 'm', ',', '.', '/', 0, '*',      // 0x30-0x37
        0, ' ', 0, 0, 0, 0, 0, 0,                  // 0x38-0x3F
    };

    static constexpr char shiftedKeymap[128] = {
        0, 0, '!', '@', '#', '$', '%', '^',        // 0x00-0x07
        '&', '*', '(', ')', '_', '+', '\b', '\t',  // 0x08-0x0F
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',    // 0x10-0x17
        'O', 'P', '{', '}', '\n', 0, 'A', 'S',     // 0x18-0x1F
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',    // 0x20-0x27
        '"', '~', 0, '|', 'Z', 'X', 'C', 'V',      // 0x28-0x2F
        'B', 'N', 'M', '<', '>', '?', 0, '*',      // 0x30-0x37
        0, ' ', 0, 0, 0, 0, 0, 0,                  // 0x38-0x3F
    };
    bool capsOn = false;
    bool shiftPressed = false;

   public:
    KeyboardEventHandler();
    ~KeyboardEventHandler();

    virtual void onKeyPress(better_os::lib::uint8_t scanCode);
    virtual void onKeyRelease(better_os::lib::uint8_t scanCode);
};

class VGAKeyboardEventHandler : public KeyboardEventHandler {
   public:
    VGAKeyboardEventHandler();
    ~VGAKeyboardEventHandler();

    void onKeyPress(better_os::lib::uint8_t scanCode);
    void onKeyRelease(better_os::lib::uint8_t scanCode);
};

class KeyboardDriver : public better_os::basics::InterruptHandler, public Driver {
    better_os::hardware::Port8Bit dataPort;
    better_os::hardware::Port8Bit commandPort;
    KeyboardEventHandler *handler;

   public:
    KeyboardDriver(better_os::basics::InterruptManager *manager, KeyboardEventHandler *handler);
    ~KeyboardDriver();
    virtual better_os::lib::uint32_t HandleInterrupt(better_os::lib::uint32_t esp);
    virtual void Activate();
    virtual better_os::lib::int32_t Reset();
    virtual void DeActivate();
};
}  // namespace drivers
}  // namespace better_os

#endif  // !__BETTER_OS_KEYBOARD_H