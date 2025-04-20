#ifndef BETTER_OS_BASICS_GDT_H
#define BETTER_OS_BASICS_GDT_H

#include "../lib/types.hpp"

namespace better_os {
namespace basics {

class GlobalDescriptorTable {
   public:
    class SegmentDescriptor {
       private:
        better_os::lib::uint16_t m_limit_lo;
        better_os::lib::uint16_t m_base_lo;
        better_os::lib::uint8_t m_base_hi;
        better_os::lib::uint8_t m_type;
        better_os::lib::uint8_t m_limit_hi;
        better_os::lib::uint8_t m_base_vhi;

       public:
        SegmentDescriptor(better_os::lib::uint32_t base,
                          better_os::lib::uint32_t limit,
                          better_os::lib::uint8_t type);
        better_os::lib::uint32_t Base();
        better_os::lib::uint32_t Limit();
    } __attribute__((packed));

   private:
    SegmentDescriptor m_nullSegmentSelector;
    SegmentDescriptor m_unusedSegmentSelector;
    SegmentDescriptor m_codeSegmentSelector;
    SegmentDescriptor m_dataSegmentSelector;

   public:
    GlobalDescriptorTable();
    ~GlobalDescriptorTable();

    better_os::lib::uint16_t CodeSegmentSelector();
    better_os::lib::uint16_t DataSegmentSelector();
};

}  // namespace basics
}  // namespace better_os

#endif  // BETTER_OS_BASICS_GDT_H
