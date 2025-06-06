#include "all.hpp"

using namespace better_os::hardware;
using namespace better_os::lib;

BaseAddressRegister::BaseAddressRegister() {}
BaseAddressRegister::~BaseAddressRegister() {}

PeripheralComponentInterconnectDeviceDescriptor::PeripheralComponentInterconnectDeviceDescriptor() {}
PeripheralComponentInterconnectDeviceDescriptor::~PeripheralComponentInterconnectDeviceDescriptor() {}

PeripheralComponentInterconnectController::PeripheralComponentInterconnectController()
    : m_dataPort(0xCFC), m_commandPort(0xCF8) {}

PeripheralComponentInterconnectController::~PeripheralComponentInterconnectController() {}

uint32_t PeripheralComponentInterconnectController::Read(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset) {
    uint32_t id = (1u << 31) | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registerOffset & 0xFC);
    m_commandPort.Write(id);
    uint32_t result = m_dataPort.Read();
    return result >> (8 * (registerOffset % 4));
}

void PeripheralComponentInterconnectController::Write(uint16_t bus, uint16_t device, uint16_t function, uint32_t registerOffset, uint32_t value) {
    uint32_t id = (1u << 31) | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11) | ((function & 0x07) << 8) | (registerOffset & 0xFC);
    m_commandPort.Write(id);
    m_dataPort.Write(value);
}

bool PeripheralComponentInterconnectController::HasFunctions(uint16_t bus, uint16_t device) {
    return Read(bus, device, 0, 0x0E) & (1 << 7);
}

PeripheralComponentInterconnectDeviceDescriptor PeripheralComponentInterconnectController::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function) {
    PeripheralComponentInterconnectDeviceDescriptor result;
    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);

    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);

    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3C);

    return result;
}

BaseAddressRegister PeripheralComponentInterconnectController::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar) {
    BaseAddressRegister result;

    uint32_t headertype = Read(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4 * headertype);
    if (bar >= maxBARs)
        return result;

    uint32_t bar_value = Read(bus, device, function, 0x10 + 4 * bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;

    if (result.type == MemoryMapping) {
        switch ((bar_value >> 1) & 0x3) {
            case 0:  // 32 Bit Mode
            case 1:  // 20 Bit Mode
            case 2:  // 64 Bit Mode
                break;
        }
    } else {  // InputOutput
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }

    return result;
}

void PeripheralComponentInterconnectController::SelectDrivers(better_os::drivers::DriverManager* driverManager, better_os::basics::InterruptManager* interruptManager) {
    for (uint8_t bus = 0; bus < 8; bus++) {
        for (uint8_t device = 0; device < 32; device++) {
            uint8_t numFunctions = HasFunctions(bus, device) ? 8 : 1;
            for (uint8_t function = 0; function < numFunctions; function++) {
                PeripheralComponentInterconnectDeviceDescriptor deviceDescriptor = GetDeviceDescriptor(bus, device, function);

                if (deviceDescriptor.vendor_id == 0x0000 || deviceDescriptor.vendor_id == 0xFFFF) {
                    continue;
                }

                for (int barNum = 0; barNum < 6; barNum++) {
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if (bar.address && (bar.type == InputOutput))
                        deviceDescriptor.portBase = (uint64_t)bar.address;
                }

                better_os::drivers::Driver* driver = 0;

                switch (deviceDescriptor.vendor_id) {
                    case 0x1022:  // AMD
                        switch (deviceDescriptor.device_id) {
                            case 0x2000:  // am79c973
                                driver = (better_os::drivers::amd_am79c973*)better_os::basics::MemoryManager::activeMemoryManager->malloc(sizeof(better_os::drivers::amd_am79c973));
                                if (driver != 0)
                                    new (driver) better_os::drivers::amd_am79c973(&deviceDescriptor, interruptManager);
                                printf("AMD AM79C973 ");
                                driverManager->netDriver = (better_os::drivers::amd_am79c973*)driver;
                                driverManager->AddDriver(driver);
                                goto exit_label;
                                break;
                        }
                        break;

                    case 0x8086:  // Intel
                        switch (deviceDescriptor.device_id) {
                            case 0x1237:
                                printf("Intel 440FX - 82441FX PMC (PCI Bridge) ");
                                break;
                            case 0x7000:
                                printf("Intel 82371SB PIIX3 ISA (ISA Bridge) ");
                                break;
                            case 0x7010:
                                printf("Intel 82371SB PIIX3 IDE (IDE Controller) ");
                                break;
                            case 0x7113:
                                printf("Intel 82371AB/EB/MB PIIX4 ACPI (Power Management Controller) ");
                                break;
                        }
                        break;
                }

                switch (deviceDescriptor.class_id) {
                    case 0x03:  // Graphics
                        switch (deviceDescriptor.subclass_id) {
                            case 0x00:  // VGA
                                printf("VGA ");
                                break;
                        }
                        break;
                }

            exit_label:

                printf("PCI -> {BUS: ");
                printhex(bus & 0xFF);
                printf(", Dev: ");
                printhex(device & 0xFF);
                printf(", Fn: ");
                printhex(function & 0xFF);
                printf("} <- by Vdr: ");
                printhex_base((deviceDescriptor.vendor_id & 0xFF00) >> 8);
                printhex_base(deviceDescriptor.vendor_id & 0xFF);
                printf(", Did: ");
                printhex_base((deviceDescriptor.device_id & 0xFF00) >> 8);
                printhex_base(deviceDescriptor.device_id & 0xFF);
                printf(".\n");
                printf("\n");
            }
        }
    }
}
