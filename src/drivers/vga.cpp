#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::drivers;

void VideoGraphicsArray_320x200x8::WriteRegisters(uint8_t* registers) {
};

uint8_t* VideoGraphicsArray_320x200x8::GetFrameBufferSegment() {
    graphicsControllerIndexPort.Write(0x06);
    uint8_t segmentNumber = graphicsControllerDataPort.Read() & (3 << 2);
    switch (segmentNumber) {
        default:
        case 0 << 2:
            return (uint8_t*)0x00000;
        case 1 << 2:
            return (uint8_t*)0xA0000;
        case 2 << 2:
            return (uint8_t*)0xB0000;
        case 3 << 2:
            return (uint8_t*)0xB0000;
    }
};

void VideoGraphicsArray_320x200x8::PutPixelUsingIndex(uint32_t x, uint32_t y, uint8_t colorIndex) {
    uint8_t* pixelAddress = GetFrameBufferSegment() + width * y + x;
    *pixelAddress = colorIndex;
};
uint8_t VideoGraphicsArray_320x200x8::GetColorIndex(uint8_t r, uint8_t g, uint8_t b) {
    if (r == 0x00 and g == 0x00 and b == 0xA8)
        return 0x01;
    return 0x00;
};

VideoGraphicsArray_320x200x8::VideoGraphicsArray_320x200x8() : miscPort(0x3C2),
                                                               crtcIndexPort(0x3D4),
                                                               crtcDataPort(0x3D5),
                                                               sequencerIndexPort(0x3C4),
                                                               sequencerDataPort(0x3C5),
                                                               graphicsControllerIndexPort(0x3CE),
                                                               graphicsControllerDataPort(0x3CF),
                                                               attributeControllerIndexPort(0x3C0),
                                                               attributeControllerReadPort(0x3C1),
                                                               attributeControllerWritePort(0x3C0),
                                                               attributeControllerResetPort(0x3DA) {}

void VideoGraphicsArray_320x200x8::SetMode() {
    // Unlock CRTC registers
    crtcIndexPort.Write(0x11);
    crtcDataPort.Write(crtcDataPort.Read() & ~0x80);

    // Set VGA mode 0x13 (320x200, 256 colors)
    miscPort.Write(0x63);

    // Sequencer registers
    sequencerIndexPort.Write(0x00);
    sequencerDataPort.Write(0x03);  // Reset
    sequencerIndexPort.Write(0x01);
    sequencerDataPort.Write(0x01);  // Clocking mode
    sequencerIndexPort.Write(0x03);
    sequencerDataPort.Write(0x00);  // Character map select
    sequencerIndexPort.Write(0x04);
    sequencerDataPort.Write(0x0E);  // Memory mode

    // CRT Controller registers
    crtcIndexPort.Write(0x00);
    crtcDataPort.Write(0x5F);  // Horizontal total
    crtcIndexPort.Write(0x01);
    crtcDataPort.Write(0x4F);  // Horizontal display enable end
    crtcIndexPort.Write(0x02);
    crtcDataPort.Write(0x50);  // Start horizontal blanking
    crtcIndexPort.Write(0x03);
    crtcDataPort.Write(0x82);  // End horizontal blanking
    crtcIndexPort.Write(0x04);
    crtcDataPort.Write(0x55);  // Start vertical retrace
    crtcIndexPort.Write(0x05);
    crtcDataPort.Write(0x81);  // Vertical total
    crtcIndexPort.Write(0x06);
    crtcDataPort.Write(0xBF);  // Overflow
    crtcIndexPort.Write(0x07);
    crtcDataPort.Write(0x1F);  // Preset row scan
    crtcIndexPort.Write(0x08);
    crtcDataPort.Write(0x00);  // Maximum scan line
    crtcIndexPort.Write(0x09);
    crtcDataPort.Write(0x4F);  // Cursor start
    crtcIndexPort.Write(0x0A);
    crtcDataPort.Write(0x0E);  // Cursor end
    crtcIndexPort.Write(0x0B);
    crtcDataPort.Write(0x00);  // Start address high
    crtcIndexPort.Write(0x0C);
    crtcDataPort.Write(0x00);  // Start address low
    crtcIndexPort.Write(0x0D);
    crtcDataPort.Write(0x00);  // Cursor location high
    crtcIndexPort.Write(0x0E);
    crtcDataPort.Write(0x00);  // Cursor location low

    // Graphics Controller registers
    graphicsControllerIndexPort.Write(0x00);
    graphicsControllerDataPort.Write(0x00);  // Set/reset
    graphicsControllerIndexPort.Write(0x01);
    graphicsControllerDataPort.Write(0x00);  // Enable set/reset
    graphicsControllerIndexPort.Write(0x02);
    graphicsControllerDataPort.Write(0x00);  // Color compare
    graphicsControllerIndexPort.Write(0x03);
    graphicsControllerDataPort.Write(0x00);  // Data rotate
    graphicsControllerIndexPort.Write(0x04);
    graphicsControllerDataPort.Write(0x00);  // Read map select
    graphicsControllerIndexPort.Write(0x05);
    graphicsControllerDataPort.Write(0x10);  // Graphics mode
    graphicsControllerIndexPort.Write(0x06);
    graphicsControllerDataPort.Write(0x0E);  // Miscellaneous
    graphicsControllerIndexPort.Write(0x07);
    graphicsControllerDataPort.Write(0x00);  // Color don't care
    graphicsControllerIndexPort.Write(0x08);
    graphicsControllerDataPort.Write(0xFF);  // Bit mask

    // Attribute Controller registers
    attributeControllerIndexPort.Write(0x10);  // Palette index
    for (uint8_t i = 0; i < 16; i++) {
        attributeControllerWritePort.Write(i);  // Set palette colors
    }
    attributeControllerIndexPort.Write(0x20);  // Enable attribute controller
}

VideoGraphicsArray_320x200x8::~VideoGraphicsArray_320x200x8() {};

void VideoGraphicsArray_320x200x8::PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    PutPixelUsingIndex(x, y, GetColorIndex(r, g, b));
};
