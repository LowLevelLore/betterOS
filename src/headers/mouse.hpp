#ifndef __BETTER_OS_MOUSE_H
#define __BETTER_OS_MOUSE_H

#include "types.hpp"
#include "interrupts.hpp"
#include "port.hpp"
#include "stdlib.hpp"

class MouseDriver : public InterruptHandler
{
    Port8Bit dataPort;
    Port8Bit commandPort;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
    int8_t col, row;

public:
    MouseDriver(InterruptManager *manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif // !__BETTER_OS_MOUSE_H