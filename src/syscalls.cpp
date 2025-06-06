#include "all.hpp"

using namespace better_os;
using namespace ::better_os::lib;

SyscallHandler::SyscallHandler(better_os::basics::InterruptManager* interruptManager, uint8_t InterruptNumber)
    : InterruptHandler(interruptManager, InterruptNumber + interruptManager->HardwareInterruptOffset()) {
}

SyscallHandler::~SyscallHandler() {
}

void printf(char*);

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp) {
    better_os::basics::CPUState* cpu = (better_os::basics::CPUState*)esp;

    switch (cpu->eax) {
        case 4:
            better_os::lib::printf((char*)cpu->ebx);
            break;

        default:
            break;
    }

    return esp;
}