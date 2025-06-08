#ifndef BETTER_OS_DRIVERS_AMD_AM79C973_H
#define BETTER_OS_DRIVERS_AMD_AM79C973_H

#include "basics/interrupts.hpp"
#include "driver.hpp"
#include "hardware/pci.hpp"
#include "hardware/port.hpp"
#include "lib/types.hpp"

namespace better_os {
namespace drivers {

class RawDataHandler {
   protected:
    amd_am79c973* backend;

   public:
    RawDataHandler(amd_am79c973* backend);
    ~RawDataHandler();

    virtual bool OnRawDataReceived(better_os::lib::uint8_t* buffer, better_os::lib::uint32_t size);
    void Send(better_os::lib::uint8_t* buffer, better_os::lib::uint32_t size);
};

class amd_am79c973 : public Driver, better_os::basics::InterruptHandler {
    struct InitializationBlock {
        better_os::lib::uint16_t mode;
        unsigned reserved1 : 4;
        unsigned numSendBuffers : 4;
        unsigned reserved2 : 4;
        unsigned numRecvBuffers : 4;
        better_os::lib::uint64_t physicalAddress : 48;
        better_os::lib::uint16_t reserved3;
        better_os::lib::uint64_t logicalAddress;
        better_os::lib::uint32_t recvBufferAddress;
        better_os::lib::uint32_t sendBufferAddress;
    } __attribute__((packed));

    struct BufferDescriptor {
        better_os::lib::uint32_t address;
        better_os::lib::uint32_t flags;
        better_os::lib::uint32_t flags2;
        better_os::lib::uint32_t avail;
    } __attribute__((packed));

    better_os::hardware::Port16Bit MACAddress0Port;
    better_os::hardware::Port16Bit MACAddress2Port;
    better_os::hardware::Port16Bit MACAddress4Port;
    better_os::hardware::Port16Bit registerDataPort;
    better_os::hardware::Port16Bit registerAddressPort;
    better_os::hardware::Port16Bit resetPort;
    better_os::hardware::Port16Bit busControlRegisterDataPort;

    InitializationBlock initBlock;

    BufferDescriptor* sendBufferDescriptor;
    better_os::lib::uint8_t sendBufferDescMemory[2 * 1024 + 15];
    better_os::lib::uint8_t sendBuffers[2 * 1024 + 15][8];
    better_os::lib::uint8_t currentSendBuffer;

    BufferDescriptor* recvBufferDescriptor;
    better_os::lib::uint8_t recvBufferDescMemory[2 * 1024 + 15];
    better_os::lib::uint8_t recvBuffers[2 * 1024 + 15][8];
    better_os::lib::uint8_t currentRecvBuffer;

    RawDataHandler* handler;

   public:
    amd_am79c973(better_os::hardware::PeripheralComponentInterconnectDeviceDescriptor* dev, better_os::basics::InterruptManager* interruptManager);
    ~amd_am79c973();

    void Activate();
    int Reset();
    better_os::lib::uint32_t HandleInterrupt(better_os::lib::uint32_t esp);

    void Send(uint8_t* buffer, int count);
    void Receive();

    void SetIPAddress(better_os::lib::uint32_t address);
    better_os::lib::uint32_t GetIPAddress();

    void SetHandler(RawDataHandler* handler);
    better_os::lib::uint64_t GetMACAddress();
};
}  // namespace drivers
}  // namespace better_os

#endif  // BETTER_OS_DRIVERS_AMD_AM79C973_H