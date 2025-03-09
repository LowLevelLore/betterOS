#ifndef __BETTER_OS_DRIVERS_VGA_H
#define __BETTER_OS_DRIVERS_VGA_H

#include "../basics/interrupts.hpp"
#include "../drivers/driver.hpp"
#include "../hardware/port.hpp"
#include "../lib/types.hpp"

namespace better_os {
namespace drivers {
class VideoGraphicsArray_320x200x8 {
   protected:
    better_os::lib::uint32_t width = 320;
    better_os::lib::uint32_t height = 200;
    better_os::lib::uint32_t depth = 8;

    hardware::Port8Bit miscPort;
    hardware::Port8Bit crtcIndexPort;
    hardware::Port8Bit crtcDataPort;
    hardware::Port8Bit sequencerIndexPort;
    hardware::Port8Bit sequencerDataPort;
    hardware::Port8Bit graphicsControllerIndexPort;
    hardware::Port8Bit graphicsControllerDataPort;
    hardware::Port8Bit attributeControllerIndexPort;
    hardware::Port8Bit attributeControllerReadPort;
    hardware::Port8Bit attributeControllerWritePort;
    hardware::Port8Bit attributeControllerResetPort;

    void WriteRegisters(better_os::lib::uint8_t* registers);
    better_os::lib::uint8_t* GetFrameBufferSegment();

    virtual void PutPixelUsingIndex(better_os::lib::uint32_t x, better_os::lib::uint32_t y, better_os::lib::uint8_t colorCode);
    virtual better_os::lib::uint8_t GetColorIndex(better_os::lib::uint8_t r, better_os::lib::uint8_t g, better_os::lib::uint8_t b);

   public:
    VideoGraphicsArray_320x200x8();
    ~VideoGraphicsArray_320x200x8();
    void SetMode();

    virtual void PutPixel(better_os::lib::uint32_t x, better_os::lib::uint32_t y, better_os::lib::uint8_t r, better_os::lib::uint8_t g, better_os::lib::uint8_t b);
    virtual void FillRectangle(better_os::lib::uint32_t x, better_os::lib::uint32_t y, better_os::lib::uint32_t w, better_os::lib::uint32_t h, better_os::lib::uint8_t r, better_os::lib::uint8_t g, better_os::lib::uint8_t b);
};
}  // namespace drivers
}  // namespace better_os

#endif  // !__BETTER_OS_DRIVERS_VGA_H