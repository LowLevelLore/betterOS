#ifndef BETTER_OS_LIB_STDLIB_H
#define BETTER_OS_LIB_STDLIB_H

#include "types.hpp"

namespace better_os {
namespace lib {

#define VGA_BASE (better_os::lib::uint16_t*)0xb8000

// Use constexpr for compile-time constants.
constexpr int MAX_ROWS = 25;
constexpr int MAX_COLS = 80;
constexpr better_os::lib::uint8_t DEFAULT_FORMATTING = 0x07;
constexpr better_os::lib::uint16_t DEFAULT_CHARACTER = ((DEFAULT_FORMATTING << 8) | ' ');

// Global state variables for VGA output.
inline better_os::lib::uint16_t* vga_base = VGA_BASE;
inline unsigned int row = 0;
inline unsigned int col = 0;

void update_cursor(int x, int y);
void clear_screen();
void scroll_screen();
void putchar(char ch);
void printf(const char* str);
void printd(better_os::lib::uint8_t integer);
void printhex(better_os::lib::uint8_t integer);
void printhex_base(better_os::lib::uint8_t integer);

static inline void outb(unsigned short int port, unsigned char value) {
    asm volatile("outb %b0, %w1" : : "a"(value), "Nd"(port));
}

}  // namespace lib
}  // namespace better_os

#endif  // BETTER_OS_LIB_STDLIB_H
