#ifndef BETTER_OS_DRIVERS_DRIVER_H
#define BETTER_OS_DRIVERS_DRIVER_H

#include "../lib/types.hpp"
#include "amd_am79c973.hpp"

namespace better_os {
namespace drivers {

class amd_am79c973;

#define MAX_DRIVERS 256

class Driver {
   public:
    char* name;

    Driver(char* name);
    ~Driver();

    virtual void Activate();
    virtual better_os::lib::int32_t Reset();
    virtual void Deactivate();
};

class DriverManager {
   private:
    Driver* m_drivers[MAX_DRIVERS];
    better_os::lib::uint8_t m_numDrivers;

   public:
    better_os::drivers::amd_am79c973* netDriver = nullptr;
    DriverManager();
    ~DriverManager();

    bool AddDriver(Driver* driver);
    bool RemoveDriver(Driver* driver);
};

}  // namespace drivers
}  // namespace better_os

#endif  // BETTER_OS_DRIVERS_DRIVER_H
