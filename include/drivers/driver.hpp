#ifndef __BETTER_OS_DRIVER_H
#define __BETTER_OS_DRIVER_H

#include "../lib/types.hpp"

namespace better_os {
namespace drivers {
#define MAX_DRIVERS 256

class Driver {
   public:
    char *name;
    Driver(char *name);
    ~Driver();

    virtual void Activate();
    virtual better_os::lib::int32_t Reset();
    virtual void DeActivate();
};

class DriverManager {
   private:
    Driver *drivers[MAX_DRIVERS];
    better_os::lib::uint8_t numDrivers;

   public:
    DriverManager();
    ~DriverManager();
    bool AddDriver(Driver *driver);
    bool RemoverDriver(Driver *driver);
};
}  // namespace drivers
}  // namespace better_os

#endif  // !__BETTER_OS_DRIVER_H