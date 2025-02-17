#ifndef __BETTER_OS_STDLIB_H
#define __BETTER_OS_STDLIB_H

#include "types.hpp"

#define VGA_BASE (uint16_t *)0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define DEFAULT_FORMATTING 0x07
#define DEFAULT_CHARACTER ((DEFAULT_FORMATTING << 8) | ' ')

static uint16_t *vga_base = (uint16_t *)VGA_BASE;
static int row = 0;
static int col = 0;

void update_cursor(int x, int y);

void clear_screen();
void scroll_screen();
void putchar(char ch);
void printf(const char *str);
void printd(uint8_t integer);
void printhex(uint8_t integer);

static inline void outb(unsigned short int port, unsigned char value)
{
    asm volatile("outb %b0,%w1" : : "a"(value), "Nd"(port));
}

#endif // !__BETTER_OS_STDLIB_H