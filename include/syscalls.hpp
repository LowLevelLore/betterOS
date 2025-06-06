#ifndef BETTER_OS_SYSCALLS_H
#define BETTER_OS_SYSCALLS_H

#include "basics/interrupts.hpp"

namespace better_os {
class SyscallHandler : public better_os::basics::InterruptHandler {
   public:
    SyscallHandler(better_os::basics::InterruptManager* interruptManager, better_os::lib::uint8_t InterruptNumber);
    ~SyscallHandler();

    virtual better_os::lib::uint32_t HandleInterrupt(better_os::lib::uint32_t esp);
};

}  // namespace better_os

#endif  // BETTER_OS_SYSCALLS_H