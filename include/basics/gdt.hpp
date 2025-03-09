#ifndef __BETTER_OS_BASICS_GDT_H
#define __BETTER_OS_BASICS_GDT_H

#include "../lib/types.hpp"

namespace better_os {
namespace basics {
class GlobalDescriptorTable {
   public:
    class SegmentDescriptor {
       private:
        better_os::lib::uint16_t limit_lo;
        better_os::lib::uint16_t base_lo;
        better_os::lib::uint8_t base_hi;
        better_os::lib::uint8_t type;
        better_os::lib::uint8_t limit_hi;
        better_os::lib::uint8_t base_vhi;

       public:
        SegmentDescriptor(better_os::lib::uint32_t base, better_os::lib::uint32_t limit, better_os::lib::uint8_t type);
        better_os::lib::uint32_t Base();
        better_os::lib::uint32_t Limit();
    } __attribute__((packed));

   private:
    SegmentDescriptor nullSegmentSelector;
    SegmentDescriptor unusedSegmentSelector;
    SegmentDescriptor codeSegmentSelector;
    SegmentDescriptor dataSegmentSelector;

   public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    better_os::lib::uint16_t CodeSegmentSelector();
    better_os::lib::uint16_t DataSegmentSelector();
};
}  // namespace basics
}  // namespace better_os

#endif  // !__BETTER_OS_BASICS_GDT_H
