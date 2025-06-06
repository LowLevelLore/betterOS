#ifndef BETTER_OS_BASICS_MEMORY_MANAGEMENT_H
#define BETTER_OS_BASICS_MEMORY_MANAGEMENT_H

#include "../lib/types.hpp"

using namespace better_os::lib;

namespace better_os {
namespace basics {
struct MemoryChunk {
    MemoryChunk *next;
    MemoryChunk *prev;
    bool allocated;
    size_t size;
};

class MemoryManager {
   protected:
    MemoryChunk *first;

   public:
    static MemoryManager *activeMemoryManager;

    MemoryManager(size_t first, size_t size);
    ~MemoryManager();

    void *malloc(size_t size);
    void free(void *ptr);
};
}  // namespace basics
}  // namespace better_os

void *operator new(unsigned size);
void *operator new[](unsigned size);

// placement new
void *operator new(unsigned size, void *ptr);
void *operator new[](unsigned size, void *ptr);

void operator delete(void *ptr);
void operator delete[](void *ptr);

#endif  // BETTER_OS_BASICS_MEMORY_MANAGEMENT_H