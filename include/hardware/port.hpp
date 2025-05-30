#ifndef __BETTER_OS_PORT_H
#define __BETTER_OS_PORT_H

#include "../lib/types.hpp"

namespace better_os {
namespace hardware {
class Port {
   protected:
    Port(better_os::lib::uint16_t portnumber);
    // FIXME: Must be virtual (currently isn't because the kernel has no memory management yet)
    ~Port();
    better_os::lib::uint16_t portnumber;
};

class Port8Bit : public Port {
   public:
    Port8Bit(better_os::lib::uint16_t portnumber);
    ~Port8Bit();

    virtual better_os::lib::uint8_t Read();
    virtual void Write(better_os::lib::uint8_t data);

   protected:
    static inline better_os::lib::uint8_t Read8(better_os::lib::uint16_t _port) {
        better_os::lib::uint8_t result;
        __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(_port));
        return result;
    }

    static inline void Write8(better_os::lib::uint16_t _port, better_os::lib::uint8_t _data) {
        __asm__ volatile("outb %0, %1" : : "a"(_data), "Nd"(_port));
    }
};

class Port8BitSlow : public Port8Bit {
   public:
    Port8BitSlow(better_os::lib::uint16_t portnumber);
    ~Port8BitSlow();

    virtual void Write(better_os::lib::uint8_t data);

   protected:
    static inline void Write8Slow(better_os::lib::uint16_t _port, better_os::lib::uint8_t _data) {
        __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a"(_data), "Nd"(_port));
    }
};

class Port16Bit : public Port {
   public:
    Port16Bit(better_os::lib::uint16_t portnumber);
    ~Port16Bit();

    virtual better_os::lib::uint16_t Read();
    virtual void Write(better_os::lib::uint16_t data);

   protected:
    static inline better_os::lib::uint16_t Read16(better_os::lib::uint16_t _port) {
        better_os::lib::uint16_t result;
        __asm__ volatile("inw %1, %0" : "=a"(result) : "Nd"(_port));
        return result;
    }

    static inline void Write16(better_os::lib::uint16_t _port, better_os::lib::uint16_t _data) {
        __asm__ volatile("outw %0, %1" : : "a"(_data), "Nd"(_port));
    }
};

class Port32Bit : public Port {
   public:
    Port32Bit(better_os::lib::uint16_t portnumber);
    ~Port32Bit();

    virtual better_os::lib::uint32_t Read();
    virtual void Write(better_os::lib::uint32_t data);

   protected:
    static inline better_os::lib::uint32_t Read32(better_os::lib::uint16_t _port) {
        better_os::lib::uint32_t result;
        __asm__ volatile("inl %1, %0" : "=a"(result) : "Nd"(_port));
        return result;
    }

    static inline void Write32(better_os::lib::uint16_t _port, better_os::lib::uint32_t _data) {
        __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd"(_port));
    }
};
}  // namespace hardware
}  // namespace better_os

#endif  // !__BETTER_OS_PORT_H
