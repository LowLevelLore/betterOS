#include "src/headers/types.hpp"
#include "src/headers/interrupts.hpp"
#include "src/headers/gdt.hpp"
#include "src/headers/keyboard.hpp"

#define VGA_BASE (uint16_t *)0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define DEFAULT_FORMATTING 0x07

static uint16_t *vga_base = (uint16_t *)VGA_BASE;
static int row = 0;
static int col = 0;

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

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
  for (constructor *i = &start_ctors; i != &end_ctors; i++)
    (*i)();
}

extern "C" void kernelMain(const void *multiboot_structure, uint32_t /*multiboot_magic*/)
{
  printf("betterOS - Dedicated to xZist\n\n");

  GlobalDescriptorTable gdt;
  InterruptManager interrupts(0x20, &gdt);

  KeyboardDriver keyboard(&interrupts);

  interrupts.Activate();

  while (1)
    ;
}