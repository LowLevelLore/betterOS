#ifndef __BETTER_OS_PCI_H
#define __BETTER_OS_PCI_H

namespace better_os {
namespace hardware {

enum BaseAddressRegisterType {
    MemoryMapping = 0,
    InputOutput = 1
};

class BaseAddressRegister {
   public:
    bool prefetchable;
    better_os::lib::uint8_t* address;
    better_os::lib::uint32_t size;
    BaseAddressRegisterType type;
    BaseAddressRegister();
    ~BaseAddressRegister();
};

class PeripheralComponentInterconnectDeviceDescriptor {
   public:
    better_os::lib::uint32_t portBase;
    better_os::lib::uint32_t interrupt;

    better_os::lib::uint16_t bus;
    better_os::lib::uint16_t device;
    better_os::lib::uint16_t function;

    better_os::lib::uint16_t vendor_id;
    better_os::lib::uint16_t device_id;

    better_os::lib::uint8_t class_id;
    better_os::lib::uint8_t subclass_id;
    better_os::lib::uint8_t interface_id;

    better_os::lib::uint8_t revision;

    PeripheralComponentInterconnectDeviceDescriptor();
    ~PeripheralComponentInterconnectDeviceDescriptor();
};

class PeripheralComponentInterconnectController {
   private:
    Port32Bit dataPort;
    Port32Bit commandPort;

   public:
    PeripheralComponentInterconnectController();
    ~PeripheralComponentInterconnectController();
    better_os::lib::uint32_t Read(better_os::lib::uint16_t bus, better_os::lib::uint16_t device, better_os::lib::uint16_t function, better_os::lib::uint32_t registeroffset);
    void Write(better_os::lib::uint16_t bus, better_os::lib::uint16_t device, better_os::lib::uint16_t function, better_os::lib::uint32_t registeroffset, better_os::lib::uint32_t value);
    bool HasFunctions(better_os::lib::uint16_t bus, better_os::lib::uint16_t device);
    void SelectDrivers(better_os::drivers::DriverManager* driverManager, better_os::basics::InterruptManager* interruptManager);
    PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(better_os::lib::uint16_t bus, better_os::lib::uint16_t device, better_os::lib::uint16_t function);
    BaseAddressRegister GetBaseAddressRegister(better_os::lib::uint16_t bus, better_os::lib::uint16_t device, better_os::lib::uint16_t function, better_os::lib::uint16_t bar);
};
}  // namespace hardware
}  // namespace better_os

#endif  // !__BETTER_OS_PCI_H