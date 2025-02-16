#include "headers/stdlib.hpp"

void clear_screen()
{
    for (int i = 0; i < MAX_ROWS * MAX_COLS; i++)
    {
        vga_base[i] = (DEFAULT_FORMATTING << 8) | ' ';
    }
    row = 0;
    col = 0;
}

void scroll_screen()
{
    for (int i = 0; i < (MAX_ROWS - 1) * MAX_COLS; i++)
    {
        vga_base[i] = vga_base[i + MAX_COLS];
    }
    for (int i = (MAX_ROWS - 1) * MAX_COLS; i < MAX_ROWS * MAX_COLS; i++)
    {
        vga_base[i] = (DEFAULT_FORMATTING << 8) | ' ';
    }
    row = MAX_ROWS - 1;
    col = 0;
}

void putchar(char ch)
{
    switch (ch)
    {
    case '\n':
        row++;
        col = 0;
        break;
    case '\t':
        col = (col + 4) & ~3;
        break;
    case '\b':
        if (col > 0)
        {
            col--;
            vga_base[row * MAX_COLS + col] = (DEFAULT_FORMATTING << 8) | ' ';
        }
        break;
    default:
        vga_base[row * MAX_COLS + col] = (DEFAULT_FORMATTING << 8) | ch;
        col++;
        break;
    }

    if (col >= MAX_COLS)
    {
        row++;
        col = 0;
    }

    if (row >= MAX_ROWS)
    {
        scroll_screen();
    }
}

void printf(const char *str)
{
    for (uint32_t i = 0; str[i] != '\0'; i++)
    {
        putchar(str[i]);
    }
}

void printd(uint8_t integer)
{
    if (integer > 9)
        printd(integer / 10);
    putchar('0' + (integer % 10));
}