#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::drivers;

Driver::Driver(char *name) : name(name) {}

Driver::~Driver() {}

DriverManager::DriverManager() {
    m_numDrivers = 0;
}

DriverManager::~DriverManager() {}

void Driver::Activate() {}

int Driver::Reset() {
    return 0;
}

void Driver::Deactivate() {}

bool DriverManager::AddDriver(Driver *driver) {
    if (m_numDrivers < MAX_DRIVERS - 1) {
        driver->Activate();
        m_drivers[m_numDrivers++] = driver;
        printf("Driver: ");
        printf(driver->name);
        printf(" added successfully.\n");
        return true;
    }
    return false;
}

bool DriverManager::RemoveDriver(Driver *driver) {
    for (uint16_t i = 0; i < MAX_DRIVERS; i++) {
        if (m_drivers[i] == driver) {
            driver->Deactivate();
            // TODO: Handle deletion; memory management is not available at this point.
            m_drivers[i] = 0;
            return true;
        }
    }
    return false;
}
