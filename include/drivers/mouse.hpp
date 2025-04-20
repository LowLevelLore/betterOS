#ifndef BETTER_OS_DRIVERS_MOUSE_H
#define BETTER_OS_DRIVERS_MOUSE_H

#include "../basics/interrupts.hpp"
#include "../hardware/port.hpp"
#include "../lib/types.hpp"
#include "driver.hpp"

namespace better_os {
namespace drivers {

class MouseEventHandler {
   protected:
    better_os::lib::int8_t m_x;
    better_os::lib::int8_t m_y;

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

    virtual void OnMouseDown(better_os::lib::uint8_t button) override;
    virtual void OnMouseUp(better_os::lib::uint8_t button) override;
    virtual void OnMouseMove(better_os::lib::int32_t deltaX, better_os::lib::int32_t deltaY) override;
};

class MouseDriver : public better_os::basics::InterruptHandler, public Driver {
   private:
    better_os::hardware::Port8Bit m_dataPort;
    better_os::hardware::Port8Bit m_commandPort;
    better_os::lib::uint8_t m_buffer[3];
    better_os::lib::uint8_t m_offset;
    better_os::lib::uint8_t m_buttons;
    MouseEventHandler* m_handler;

   public:
    MouseDriver(better_os::basics::InterruptManager* manager, MouseEventHandler* handler);
    ~MouseDriver();

    virtual better_os::lib::uint32_t HandleInterrupt(better_os::lib::uint32_t esp) override;
    virtual void Activate() override;
    virtual better_os::lib::int32_t Reset() override;
    virtual void Deactivate() override;
};

}  // namespace drivers
}  // namespace better_os

#endif  // BETTER_OS_DRIVERS_MOUSE_H
