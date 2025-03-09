#ifndef __BETTER_OS_DRIVERS_MOUSE_H
#define __BETTER_OS_DRIVERS_MOUSE_H

#include "../basics/interrupts.hpp"
#include "../hardware/port.hpp"
#include "../lib/types.hpp"
#include "driver.hpp"

namespace better_os {
namespace drivers {
class MouseEventHandler {
   protected:
    better_os::lib::int8_t x;
    better_os::lib::int8_t y;

   public:
    MouseEventHandler();
    ~MouseEventHandler();

    virtual void OnMouseDown(better_os::lib::uint8_t button);
    virtual void OnMouseUp(better_os::lib::uint8_t button);
    virtual void OnMouseMove(better_os::lib::int32_t deltaX, better_os::lib::int32_t deltaY);
};

class VGAMouseEventHandler : public MouseEventHandler {
   public:
    VGAMouseEventHandler();
    ~VGAMouseEventHandler();
    virtual void OnMouseDown(better_os::lib::uint8_t button);
    virtual void OnMouseUp(better_os::lib::uint8_t button);
    virtual void OnMouseMove(better_os::lib::int32_t deltaX, better_os::lib::int32_t deltaY);
};

class MouseDriver : public better_os::basics::InterruptHandler, public Driver {
    better_os::hardware::Port8Bit dataPort;
    better_os::hardware::Port8Bit commandPort;
    better_os::lib::uint8_t buffer[3];
    better_os::lib::uint8_t offset;
    better_os::lib::uint8_t buttons;
    MouseEventHandler *handler;

   public:
    MouseDriver(better_os::basics::InterruptManager *manager, MouseEventHandler *handler);
    ~MouseDriver();
    virtual better_os::lib::uint32_t HandleInterrupt(better_os::lib::uint32_t esp);
    virtual void Activate();
    virtual better_os::lib::int32_t Reset();
    virtual void DeActivate();
};
}  // namespace drivers
}  // namespace better_os

#endif  // !__BETTER_OS_DRIVERS_MOUSE_H