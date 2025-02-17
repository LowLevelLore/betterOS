#ifndef __BETTER_OS_MOUSE_H
#define __BETTER_OS_MOUSE_H

#include "../lib/types.hpp"
#include "../basics/interrupts.hpp"
#include "../hardware/port.hpp"
#include "driver.hpp"

class MouseEventHandler
{
protected:
    int8_t x;
    int8_t y;

public:
    MouseEventHandler();
    ~MouseEventHandler();

    virtual void OnMouseDown(uint8_t button);
    virtual void OnMouseUp(uint8_t button);
    virtual void OnMouseMove(int32_t deltaX, int32_t deltaY);
};

class VGAMouseEventHandler : public MouseEventHandler
{
public:
    VGAMouseEventHandler();
    ~VGAMouseEventHandler();
    virtual void OnMouseDown(uint8_t button);
    virtual void OnMouseUp(uint8_t button);
    virtual void OnMouseMove(int32_t deltaX, int32_t deltaY);
};

class MouseDriver : public InterruptHandler, public Driver
{
    Port8Bit dataPort;
    Port8Bit commandPort;
    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;
    MouseEventHandler *handler;

public:
    MouseDriver(InterruptManager *manager, MouseEventHandler *handler);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
    virtual int32_t Reset();
    virtual void DeActivate();
};

#endif // !__BETTER_OS_MOUSE_H