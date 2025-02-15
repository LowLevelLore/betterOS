#ifndef __BETTER_OS_KEYBOARD_H
#define __BETTER_OS_KEYBOARD_H

#include "types.hpp"
#include "interrupts.hpp"
#include "port.hpp"

class KeyboardDriver : public InterruptHandler
{
protected:
    Port8Bit dataPort;
    Port8Bit commandPort;

public:
    KeyboardDriver(InterruptManager *interruptManager);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif // !__BETTER_OS_KEYBOARD_H