#include "all.hpp"

void better_os::lib::clear_screen() {
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++) {
        vga_base[i] = DEFAULT_CHARACTER;
    }
    row = 0;
    col = 0;
}

void better_os::lib::scroll_screen() {
    for (int i = 0; i < (MAX_ROWS - 1) * MAX_COLS; i++) {
        vga_base[i] = vga_base[i + MAX_COLS];
    }
    for (int i = (MAX_ROWS - 1) * MAX_COLS; i < MAX_ROWS * MAX_COLS; i++) {
        vga_base[i] = DEFAULT_CHARACTER;
    }
    row = MAX_ROWS - 1;
    col = 0;
}

void better_os::lib::update_cursor(int x, int y) {
    uint16_t pos = y * 80 + x;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void better_os::lib::putchar(char ch) {
    switch (ch) {
        case '\n':
            row++;
            col = 0;
            break;
        case '\t':
            col = (col + 4) & ~3;
            break;
        case '\b':
            if (row == 0 and col == 0) {
                break;
            }
            if (col > 0) {
                col--;
                vga_base[row * MAX_COLS + col] = DEFAULT_CHARACTER;
            } else {
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

void better_os::lib::printf(const char *str) {
    for (uint32_t i = 0; str[i] != '\0'; i++) {
        putchar(str[i]);
    }
}

void better_os::lib::printd(uint8_t integer) {
    if (integer >= 10)
        printd(integer / 10);
    putchar('0' + (integer % 10));
}

void better_os::lib::printhex(uint8_t integer) {
    printf("0x");
    printhex_base(integer);
}

void better_os::lib::printhex_base(uint8_t integer) {
    char msg[3];  // Writable stack-allocated buffer
    char *hex = "0123456789ABCDEF";
    msg[0] = hex[(integer >> 4) & 0xF];
    msg[1] = hex[integer & 0xF];
    msg[2] = '\0';  // Null-terminate
    printf(msg);
}