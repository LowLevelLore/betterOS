#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::drivers;

uint8_t* VideoGraphicsArray_320x200x8::GetFrameBufferSegment() {
    m_graphicsControllerIndexPort.Write(0x06);
    uint8_t segmentNumber = m_graphicsControllerDataPort.Read() & (3 << 2);
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
}

void VideoGraphicsArray_320x200x8::PutPixelUsingIndex(uint32_t x, uint32_t y, uint8_t colorIndex) {
    uint8_t* pixelAddress = GetFrameBufferSegment() + m_width * y + x;
    *pixelAddress = colorIndex;
}

uint8_t VideoGraphicsArray_320x200x8::GetColorIndex(uint8_t r, uint8_t g, uint8_t b) {
    if (r == 0x00 && g == 0x00 && b == 0xA8)
        return 0x01;
    return 0x00;
}

VideoGraphicsArray_320x200x8::VideoGraphicsArray_320x200x8()
    : m_miscPort(0x3C2),
      m_crtcIndexPort(0x3D4),
      m_crtcDataPort(0x3D5),
      m_sequencerIndexPort(0x3C4),
      m_sequencerDataPort(0x3C5),
      m_graphicsControllerIndexPort(0x3CE),
      m_graphicsControllerDataPort(0x3CF),
      m_attributeControllerIndexPort(0x3C0),
      m_attributeControllerReadPort(0x3C1),
      m_attributeControllerWritePort(0x3C0),
      m_attributeControllerResetPort(0x3DA) {
    // m_width, m_height, m_depth are set by default in the header.
}

void VideoGraphicsArray_320x200x8::SetMode() {
    // Unlock CRTC registers
    m_crtcIndexPort.Write(0x11);
    m_crtcDataPort.Write(m_crtcDataPort.Read() & ~0x80);

    // Set VGA mode 0x13 (320x200, 256 colors)
    m_miscPort.Write(0x63);

    // Sequencer registers
    m_sequencerIndexPort.Write(0x00);
    m_sequencerDataPort.Write(0x03);  // Reset
    m_sequencerIndexPort.Write(0x01);
    m_sequencerDataPort.Write(0x01);  // Clocking mode
    m_sequencerIndexPort.Write(0x03);
    m_sequencerDataPort.Write(0x00);  // Character map select
    m_sequencerIndexPort.Write(0x04);
    m_sequencerDataPort.Write(0x0E);  // Memory mode

    // CRT Controller registers
    m_crtcIndexPort.Write(0x00);
    m_crtcDataPort.Write(0x5F);  // Horizontal total
    m_crtcIndexPort.Write(0x01);
    m_crtcDataPort.Write(0x4F);  // Horizontal display enable end
    m_crtcIndexPort.Write(0x02);
    m_crtcDataPort.Write(0x50);  // Start horizontal blanking
    m_crtcIndexPort.Write(0x03);
    m_crtcDataPort.Write(0x82);  // End horizontal blanking
    m_crtcIndexPort.Write(0x04);
    m_crtcDataPort.Write(0x55);  // Start vertical retrace
    m_crtcIndexPort.Write(0x05);
    m_crtcDataPort.Write(0x81);  // Vertical total
    m_crtcIndexPort.Write(0x06);
    m_crtcDataPort.Write(0xBF);  // Overflow
    m_crtcIndexPort.Write(0x07);
    m_crtcDataPort.Write(0x1F);  // Preset row scan
    m_crtcIndexPort.Write(0x08);
    m_crtcDataPort.Write(0x00);  // Maximum scan line
    m_crtcIndexPort.Write(0x09);
    m_crtcDataPort.Write(0x4F);  // Cursor start
    m_crtcIndexPort.Write(0x0A);
    m_crtcDataPort.Write(0x0E);  // Cursor end
    m_crtcIndexPort.Write(0x0B);
    m_crtcDataPort.Write(0x00);  // Start address high
    m_crtcIndexPort.Write(0x0C);
    m_crtcDataPort.Write(0x00);  // Start address low
    m_crtcIndexPort.Write(0x0D);
    m_crtcDataPort.Write(0x00);  // Cursor location high
    m_crtcIndexPort.Write(0x0E);
    m_crtcDataPort.Write(0x00);  // Cursor location low

    // Graphics Controller registers
    m_graphicsControllerIndexPort.Write(0x00);
    m_graphicsControllerDataPort.Write(0x00);  // Set/reset
    m_graphicsControllerIndexPort.Write(0x01);
    m_graphicsControllerDataPort.Write(0x00);  // Enable set/reset
    m_graphicsControllerIndexPort.Write(0x02);
    m_graphicsControllerDataPort.Write(0x00);  // Color compare
    m_graphicsControllerIndexPort.Write(0x03);
    m_graphicsControllerDataPort.Write(0x00);  // Data rotate
    m_graphicsControllerIndexPort.Write(0x04);
    m_graphicsControllerDataPort.Write(0x00);  // Read map select
    m_graphicsControllerIndexPort.Write(0x05);
    m_graphicsControllerDataPort.Write(0x10);  // Graphics mode
    m_graphicsControllerIndexPort.Write(0x06);
    m_graphicsControllerDataPort.Write(0x0E);  // Miscellaneous
    m_graphicsControllerIndexPort.Write(0x07);
    m_graphicsControllerDataPort.Write(0x00);  // Color don't care
    m_graphicsControllerIndexPort.Write(0x08);
    m_graphicsControllerDataPort.Write(0xFF);  // Bit mask

    // Attribute Controller registers
    m_attributeControllerIndexPort.Write(0x10);  // Palette index
    for (uint8_t i = 0; i < 16; i++) {
        m_attributeControllerWritePort.Write(i);  // Set palette colors
    }
    m_attributeControllerIndexPort.Write(0x20);  // Enable attribute controller
}

VideoGraphicsArray_320x200x8::~VideoGraphicsArray_320x200x8() {}

void VideoGraphicsArray_320x200x8::FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint8_t r, uint8_t g, uint8_t b) {
    for (uint32_t j = y; j < y + h; j++) {
        for (uint32_t i = x; i < x + w; i++) {
            PutPixel(i, j, r, g, b);
        }
    }
}

void VideoGraphicsArray_320x200x8::PutPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b) {
    PutPixelUsingIndex(x, y, GetColorIndex(r, g, b));
}
