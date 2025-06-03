#ifndef BETTER_OS_BASICS_MULTITASKING_H
#define BETTER_OS_BASICS_MULTITASKING_H

#include "../lib/types.hpp"
#include "gdt.hpp"

namespace better_os {
namespace basics {
struct CPUState {
    better_os::lib::uint32_t eax;
    better_os::lib::uint32_t ebx;
    better_os::lib::uint32_t ecx;
    better_os::lib::uint32_t edx;

    better_os::lib::uint32_t esi;
    better_os::lib::uint32_t edi;
    better_os::lib::uint32_t ebp;

    /*
    better_os::lib::uint32_t gs;
    better_os::lib::uint32_t fs;
    better_os::lib::uint32_t es;
    better_os::lib::uint32_t ds;
    */
    better_os::lib::uint32_t error;

    better_os::lib::uint32_t eip;
    better_os::lib::uint32_t cs;
    better_os::lib::uint32_t eflags;
    better_os::lib::uint32_t esp;
    better_os::lib::uint32_t ss;
} __attribute__((packed));

class Task {
    friend class TaskManager;

   private:
    better_os::lib::uint8_t stack[4096];  // 4 KiB
    CPUState* cpustate;

   public:
    Task(GlobalDescriptorTable* gdt, void entrypoint());
    ~Task();
};

class TaskManager {
   private:
    Task* tasks[256];
    int numTasks;
    int currentTask;

   public:
    TaskManager();
    ~TaskManager();
    bool AddTask(Task* task);
    CPUState* Schedule(CPUState* cpustate);
};
}  // namespace basics
}  // namespace better_os

#endif  // BETTER_OS_BASICS_MULTITASKING_H