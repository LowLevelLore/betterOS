#include "all.hpp"

Driver::Driver(char *name) : name(name) {
                             };

Driver::~Driver() {
};

DriverManager::DriverManager()
{
    numDrivers = 0;
}

DriverManager::~DriverManager() {

};

void Driver::Activate()
{
}
int Driver::Reset()
{
    return 0;
}
void Driver::DeActivate()
{
}

bool DriverManager::AddDriver(Driver *driver)
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

bool DriverManager::RemoverDriver(Driver *driver)
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
