#ifndef BETTER_OS_DRIVERS_ATA_H
#define BETTER_OS_DRIVERS_ATA_H

#include "hardware/port.hpp"

namespace better_os {
namespace drivers {
class AdvancedTechnologyAttachment {
   protected:
    bool master;
    better_os::hardware::Port16Bit dataPort;
    better_os::hardware::Port8Bit errorPort;
    better_os::hardware::Port8Bit sectorCountPort;
    better_os::hardware::Port8Bit lbaLowPort;
    better_os::hardware::Port8Bit lbaMidPort;
    better_os::hardware::Port8Bit lbaHighPort;
    better_os::hardware::Port8Bit devicePort;
    better_os::hardware::Port8Bit commandPort;
    better_os::hardware::Port8Bit controlPort;

   public:
    AdvancedTechnologyAttachment(bool master, better_os::lib::uint16_t portBase);
    ~AdvancedTechnologyAttachment();

    void* Identify();
    char* Read28(better_os::lib::uint32_t sectorNum, int count = 512);
    void Write28(better_os::lib::uint32_t sectorNum, better_os::lib::uint8_t* data, better_os::lib::uint32_t count);
    void Flush();
};
}  // namespace drivers
}  // namespace better_os

#endif  // BETTER_OS_DRIVERS_ATA_H