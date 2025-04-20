#include "all.hpp"

namespace better_os {
namespace lib {

void clear_screen() {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        vga_base[i] = DEFAULT_CHARACTER;
    }
    row = 0;
    col = 0;
}

void scroll_screen() {
    for (int i = 0; i < (MAX_ROWS - 1) * MAX_COLS; i++) {
        vga_base[i] = vga_base[i + MAX_COLS];
    }
    for (int i = (MAX_ROWS - 1) * MAX_COLS; i < MAX_ROWS * MAX_COLS; i++) {
        vga_base[i] = DEFAULT_CHARACTER;
    }
    row = MAX_ROWS - 1;
    col = 0;
}

void update_cursor(int x, int y) {
    uint16_t pos = y * MAX_COLS + x;  // Using MAX_COLS for consistency.
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void putchar(char ch) {
    switch (ch) {
        case '\n':
            row++;
            col = 0;
            break;
        case '\t':
            col = (col + 4) & ~3;
            break;
        case '\b':
            if (row == 0 && col == 0) {
                break;
            }
            if (col > 0) [[likely]] {
                col--;
                vga_base[row * MAX_COLS + col] = DEFAULT_CHARACTER;
            } else [[unlikely]] {
                row--;
                col = MAX_COLS - 1;
                while (vga_base[row * MAX_COLS + col] == DEFAULT_CHARACTER) {
                    col--;
                }
                vga_base[row * MAX_COLS + col] = DEFAULT_CHARACTER;
                if (row < 0) row = 0;
                if (col < 0) col = 0;
            }
            break;
        default:
            vga_base[row * MAX_COLS + col] = (DEFAULT_FORMATTING << 8) | ch;
            col++;
            break;
    }

    if (col >= MAX_COLS) {
        row++;
        col = 0;
    }

    if (row >= MAX_ROWS) {
        scroll_screen();
    }

    update_cursor(col, row);
}

void printf(const char *str) {
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}

void printd(uint8_t integer) {
    if (integer >= 10)
        printd(integer / 10);
    putchar('0' + (integer % 10));
}

void printhex(uint8_t integer) {
    printf("0x");
    printhex_base(integer);
}

void printhex_base(uint8_t integer) {
    char msg[3];  // Writable stack-allocated buffer.
    const char *hex = "0123456789ABCDEF";
    msg[0] = hex[(integer >> 4) & 0xF];
    msg[1] = hex[integer & 0xF];
    msg[2] = '\0';  // Null-terminate.
    printf(msg);
}

}  // namespace lib
}  // namespace better_os
