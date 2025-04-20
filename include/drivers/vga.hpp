#ifndef BETTER_OS_DRIVERS_VGA_H
#define BETTER_OS_DRIVERS_VGA_H

#include "../basics/interrupts.hpp"
#include "../drivers/driver.hpp"
#include "../hardware/port.hpp"
#include "../lib/types.hpp"

namespace better_os {
namespace drivers {

class VideoGraphicsArray_320x200x8 {
   protected:
    better_os::lib::uint32_t m_width = 320;
    better_os::lib::uint32_t m_height = 200;
    better_os::lib::uint32_t m_depth = 8;

    better_os::hardware::Port8Bit m_miscPort;
    better_os::hardware::Port8Bit m_crtcIndexPort;
    better_os::hardware::Port8Bit m_crtcDataPort;
    better_os::hardware::Port8Bit m_sequencerIndexPort;
    better_os::hardware::Port8Bit m_sequencerDataPort;
    better_os::hardware::Port8Bit m_graphicsControllerIndexPort;
    better_os::hardware::Port8Bit m_graphicsControllerDataPort;
    better_os::hardware::Port8Bit m_attributeControllerIndexPort;
    better_os::hardware::Port8Bit m_attributeControllerReadPort;
    better_os::hardware::Port8Bit m_attributeControllerWritePort;
    better_os::hardware::Port8Bit m_attributeControllerResetPort;

    void WriteRegisters(better_os::lib::uint8_t* registers);
    better_os::lib::uint8_t* GetFrameBufferSegment();

    virtual void PutPixelUsingIndex(better_os::lib::uint32_t x,
                                    better_os::lib::uint32_t y,
                                    better_os::lib::uint8_t colorCode);
    virtual better_os::lib::uint8_t GetColorIndex(better_os::lib::uint8_t r,
                                                  better_os::lib::uint8_t g,
                                                  better_os::lib::uint8_t b);

   public:
    VideoGraphicsArray_320x200x8();
    ~VideoGraphicsArray_320x200x8();
    void SetMode();

    virtual void PutPixel(better_os::lib::uint32_t x,
                          better_os::lib::uint32_t y,
                          better_os::lib::uint8_t r,
                          better_os::lib::uint8_t g,
                          better_os::lib::uint8_t b);
    virtual void FillRectangle(better_os::lib::uint32_t x,
                               better_os::lib::uint32_t y,
                               better_os::lib::uint32_t w,
                               better_os::lib::uint32_t h,
                               better_os::lib::uint8_t r,
                               better_os::lib::uint8_t g,
                               better_os::lib::uint8_t b);
};

}  // namespace drivers
}  // namespace better_os

#endif  // BETTER_OS_DRIVERS_VGA_H
