#ifndef __BETTER_OS_DRIVER_H
#define __BETTER_OS_DRIVER_H

#include "../lib/types.hpp"

#define MAX_DRIVERS 256

class Driver
{
public:
    char *name;
    Driver(char *name);
    ~Driver();

    virtual void Activate();
    virtual int32_t Reset();
    virtual void DeActivate();
};

class DriverManager
{
private:
    Driver *drivers[MAX_DRIVERS];
    uint8_t numDrivers;

public:
    DriverManager();
    ~DriverManager();
    bool AddDriver(Driver *driver);
    bool RemoverDriver(Driver *driver);
};

#endif // !__BETTER_OS_DRIVER_H