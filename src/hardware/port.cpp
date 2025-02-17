#include "all.hpp"

using namespace better_os::lib;

better_os::hardware::Port::Port(uint16_t portnumber)
{
  this->portnumber = portnumber;
}

better_os::hardware::Port::~Port()
{
}

better_os::hardware::Port8Bit::Port8Bit(uint16_t portnumber)
    : Port(portnumber)
{
}

better_os::hardware::Port8Bit::~Port8Bit()
{
}

void better_os::hardware::Port8Bit::Write(uint8_t data)
{
  Write8(portnumber, data);
}

uint8_t better_os::hardware::Port8Bit::Read()
{
  return Read8(portnumber);
}

better_os::hardware::Port8BitSlow::Port8BitSlow(uint16_t portnumber)
    : Port8Bit(portnumber)
{
}

better_os::hardware::Port8BitSlow::~Port8BitSlow()
{
}

void better_os::hardware::Port8BitSlow::Write(uint8_t data)
{
  Write8Slow(portnumber, data);
}

better_os::hardware::Port16Bit::Port16Bit(uint16_t portnumber)
    : Port(portnumber)
{
}

better_os::hardware::Port16Bit::~Port16Bit()
{
}

void better_os::hardware::Port16Bit::Write(uint16_t data)
{
  Write16(portnumber, data);
}

uint16_t better_os::hardware::Port16Bit::Read()
{
  return Read16(portnumber);
}

better_os::hardware::Port32Bit::Port32Bit(uint16_t portnumber)
    : Port(portnumber)
{
}

better_os::hardware::Port32Bit::~Port32Bit()
{
}

void better_os::hardware::Port32Bit::Write(uint32_t data)
{
  Write32(portnumber, data);
}

uint32_t better_os::hardware::Port32Bit::Read()
{
  return Read32(portnumber);
}