#include "headers/keyboard.hpp"
#include "headers/stdlib.hpp"

void printf(const char *str);
void printd(uint8_t integer);

KeyboardDriver::KeyboardDriver(InterruptManager *manager)
    : InterruptHandler(manager, 0x21),
      dataport(0x60),
      commandport(0x64)
{
    while (commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xae); // activate interrupts
    commandport.Write(0x20); // command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // command 0x60 = set controller command byte
    dataport.Write(status);
    dataport.Write(0xf4);
}

KeyboardDriver::~KeyboardDriver()
{
}

// uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
// {
//     uint8_t key = dataport.Read();
//     switch (key)
//     {
//     case 0x2A:
//     case 0x36:
//         shiftPressed = true;
//         break;
//     case 0xAA:
//     case 0xB6:
//         shiftPressed = false;
//         break;
//     case 0x3A:
//         capsOn = not capsOn;
//         break;
//     case 0x30:
//         putchar(' ');
//         break;
//     case 0x0F:
//         putchar('\t');
//         break;
//     default:
//         break;
//     }

//     return esp;
// }

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    static bool extendedKey = false; // Track if the next scancode is an extended key
    uint8_t key = dataport.Read();

    if (key == 0xE0)
    {
        extendedKey = true; // Next scancode is part of an extended key
        return esp;
    }

    switch (key)
    {
    case 0x2A: // Left Shift pressed
    case 0x36: // Right Shift pressed
        shiftPressed = true;
        break;
    case 0xAA: // Left Shift released
    case 0xB6: // Right Shift released
        shiftPressed = false;
        break;
    case 0x3A: // Caps Lock toggle
        capsOn = !capsOn;
        break;
    case 0x39: // Space
        putchar(' ');
        break;
    case 0x0F: // Tab
        putchar('\t');
        break;
    case 0x1C: // Enter
        putchar('\n');
        break;
    case 0x0E: // Backspace
        putchar('\b');
        break;
    default:
        if (key < 0x80)
        {
            if (extendedKey)
            {
                switch (key)
                {
                case 0x48:
                    putchar('^');
                    break; // Up Arrow
                case 0x50:
                    putchar('v');
                    break; // Down Arrow
                case 0x4B:
                    putchar('<');
                    break; // Left Arrow
                case 0x4D:
                    putchar('>');
                    break; // Right Arrow
                case 0x52:
                    printf("Ins\t");
                    break; // Insert
                case 0x53:
                    printf("Del\t");
                    break; // Delete
                case 0x47:
                    printf("Hom\t");
                    break; // Home
                case 0x4F:
                    printf("End\t");
                    break; // End
                case 0x49:
                    printf("Pup\t");
                    break; // Page Up
                case 0x51:
                    printf("Pdn\t");
                    break; // Page Down
                default:
                    break;
                }
                extendedKey = false;
            }
            else
            {
                if (key < sizeof(keymap))
                {
                    char ch = shiftPressed ? shiftedKeymap[key] : keymap[key];
                    if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
                    {
                        bool effectiveShift = shiftPressed ^ capsOn;
                        ch = effectiveShift ? shiftedKeymap[key] : keymap[key];
                    }
                    if (ch != 0)
                    {
                        putchar(ch);
                    }
                }
            }
        }
        break;
    }
    return esp;
}
