#include "all.hpp"

using namespace better_os::lib;

better_os::drivers::Driver::Driver(char *name) : name(name) {
                                                 };

better_os::drivers::Driver::~Driver() {
};

better_os::drivers::DriverManager::DriverManager()
{
    numDrivers = 0;
}

better_os::drivers::DriverManager::~DriverManager() {

};

void better_os::drivers::Driver::Activate()
{
}
int better_os::drivers::Driver::Reset()
{
    return 0;
}
void better_os::drivers::Driver::DeActivate()
{
}

bool better_os::drivers::DriverManager::AddDriver(Driver *driver)
{
    if (numDrivers < MAX_DRIVERS - 1)
    {
        driver->Activate();
        drivers[numDrivers++] = driver;
        printf("Driver: ");
        printf(driver->name);
        printf(" added successfully.\n");
        return true;
    }
    return false;
};

bool better_os::drivers::DriverManager::RemoverDriver(Driver *driver)
{
    for (uint16_t i = 0; i < 256; i++)
    {
        if (drivers[i] == driver)
        {
            driver->DeActivate();
            // TODO: Do handle deleting, I dont have free at this point....
            drivers[i] = 0;
            return true;
        }
    }
    return false;
};
