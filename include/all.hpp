#ifndef BETTER_OS_ALL_H
#define BETTER_OS_ALL_H

// Basic system definitions and operations.
#include "basics/gdt.hpp"
#include "basics/interrupts.hpp"

// Device drivers.
#include "drivers/driver.hpp"
#include "drivers/keyboard.hpp"
#include "drivers/mouse.hpp"
#include "drivers/vga.hpp"

// Hardware interfaces.
#include "hardware/pci.hpp"
#include "hardware/port.hpp"

// Core libraries.
#include "lib/stdlib.hpp"
#include "lib/types.hpp"

// Graphical user interface components.
#include "gui/desktop.hpp"
#include "gui/widget.hpp"
#include "gui/window.hpp"

#endif  // BETTER_OS_ALL_H
