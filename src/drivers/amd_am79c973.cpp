#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::drivers;

RawDataHandler::RawDataHandler(amd_am79c973 *backend) {
    this->backend = backend;
    backend->SetHandler(this);
}

RawDataHandler::~RawDataHandler() {
    backend->SetHandler(0);
}

bool RawDataHandler::OnRawDataReceived(uint8_t *buffer, uint32_t size) {
    return false;
}

void RawDataHandler::Send(uint8_t *buffer, uint32_t size) {
    backend->Send(buffer, size);
}

amd_am79c973::amd_am79c973(
    better_os::hardware::PeripheralComponentInterconnectDeviceDescriptor *dev,
    better_os::basics::InterruptManager *interruptManager)
    : Driver("NIC: AMD A79C973"),
      InterruptHandler(interruptManager,
                       dev->interrupt +
                           interruptManager->HardwareInterruptOffset()),
      MACAddress0Port(dev->portBase),
      MACAddress2Port(dev->portBase + 0x02),
      MACAddress4Port(dev->portBase + 0x04),
      registerDataPort(dev->portBase + 0x10),
      registerAddressPort(dev->portBase + 0x12),
      resetPort(dev->portBase + 0x14),
      busControlRegisterDataPort(dev->portBase + 0x16) {
    this->handler = nullptr;
    currentSendBuffer = 0;
    currentRecvBuffer = 0;

    uint8_t MAC0 = MACAddress0Port.Read() % 256;
    uint8_t MAC1 = MACAddress0Port.Read() / 256;
    uint8_t MAC2 = MACAddress2Port.Read() % 256;
    uint8_t MAC3 = MACAddress2Port.Read() / 256;
    uint8_t MAC4 = MACAddress4Port.Read() % 256;
    uint8_t MAC5 = MACAddress4Port.Read() / 256;

    uint64_t MAC = ((uint64_t)MAC5 << 40) | ((uint64_t)MAC4 << 32) | ((uint64_t)MAC3 << 24) |
                   ((uint64_t)MAC2 << 16) | ((uint64_t)MAC1 << 8) | (uint64_t)MAC0;

    // 32 bit mode
    registerAddressPort.Write(20);
    busControlRegisterDataPort.Write(0x102);

    // Stop Reset
    registerAddressPort.Write(0);
    registerDataPort.Write(0x04);

    // Initialization Block
    initBlock.mode = 0x0000;  // promiscuous mode = false;
    initBlock.reserved1 = 0;
    initBlock.numSendBuffers = 3;
    initBlock.reserved2 = 0;
    initBlock.numRecvBuffers = 3;
    initBlock.physicalAddress = MAC;
    initBlock.reserved3 = 0;
    initBlock.logicalAddress = 0;

    sendBufferDescriptor = (BufferDescriptor *)((((uint32_t)&(sendBufferDescMemory[0]) + 15) & ~((uint32_t)0xF)));
    initBlock.sendBufferAddress = (uint32_t)sendBufferDescriptor;

    recvBufferDescriptor = (BufferDescriptor *)((((uint32_t)&(recvBufferDescMemory[0]) + 15) & ~((uint32_t)0xF)));
    initBlock.recvBufferAddress = (uint32_t)recvBufferDescriptor;

    for (uint8_t i = 0; i < 8; i++) {
        sendBufferDescriptor[i].address = (((uint32_t)&sendBuffers[i]) + 15) & ~(uint32_t)0xF;
        sendBufferDescriptor[i].flags = 0x7FF | 0xF000;
        sendBufferDescriptor[i].flags2 = 0;
        sendBufferDescriptor[i].avail = 0;

        recvBufferDescriptor[i].address = (((uint32_t)&recvBuffers[i]) + 15) & ~(uint32_t)0xF;
        recvBufferDescriptor[i].flags = 0x80000000 | (2048 & 0xFFF);
        recvBufferDescriptor[i].flags2 = 0;
        recvBufferDescriptor[i].avail = 0;
    }

    registerAddressPort.Write(1);
    registerDataPort.Write((uint32_t)(&initBlock) & 0xFFFF);
    registerAddressPort.Write(2);
    registerDataPort.Write(((uint32_t)(&initBlock) >> 16) & 0xFFFF);
}

amd_am79c973::~amd_am79c973() {}

void amd_am79c973::Activate() {
    registerAddressPort.Write(0);
    registerDataPort.Write(0x41);

    registerAddressPort.Write(4);
    uint32_t temp = registerDataPort.Read();
    registerAddressPort.Write(4);
    registerDataPort.Write(temp | 0xC00);

    registerAddressPort.Write(0);
    registerDataPort.Write(0x42);
}

int amd_am79c973::Reset() {
    resetPort.Read();
    resetPort.Write(0);
    return 10;
}

uint32_t
amd_am79c973::HandleInterrupt(uint32_t esp) {
    printf("INTERRUPT FROM AND AM7C973\n");

    registerAddressPort.Write(0);
    uint32_t temp = registerDataPort.Read();

    if ((temp & 0x8000) == 0x8000) printf("AMD AM7C973 ERROR\n");
    if ((temp & 0x2000) == 0x2000) printf("AMD AM7C973 COLLISION ERROR\n");
    if ((temp & 0x1000) == 0x1000) printf("AMD AM7C973 MISSED FRAME\n");
    if ((temp & 0x0800) == 0x0800) printf("AMD AM7C973 MEMORY ERROR\n");
    if ((temp & 0x0400) == 0x0400) Receive();
    if ((temp & 0x0200) == 0x0200) printf("AMD AM7C973 DATA SENT\n");

    // ACK
    registerAddressPort.Write(0);
    registerDataPort.Write(temp);

    if ((temp & 0x0100) == 0x0100) printf("AMD am79c973 INIT DONE\n");

    return esp;
}

void amd_am79c973::Send(uint8_t *buffer, int size) {
    int sendDescriptor = currentSendBuffer;
    currentSendBuffer = (currentSendBuffer + 1) % 8;

    if (size > 1518)
        size = 1518;

    for (uint8_t *src = buffer + size - 1,
                 *dst = (uint8_t *)(sendBufferDescriptor[sendDescriptor].address + size - 1);
         src >= buffer; src--, dst--)
        *dst = *src;

    printf("Sending: ");
    for (int i = 0; i < size; i++) {
        better_os::lib::printhex(buffer[i]);
        printf(" ");
    }
    // better_os::lib::printf((char *)buffer);

    sendBufferDescriptor[sendDescriptor].avail = 0;
    sendBufferDescriptor[sendDescriptor].flags2 = 0;
    sendBufferDescriptor[sendDescriptor].flags = 0x8300F000 | ((uint16_t)((-size) & 0xFFF));
    registerAddressPort.Write(0);
    registerDataPort.Write(0x48);
}

void amd_am79c973::Receive() {
    printf("AMD am79c973 DATA RECEIVED\n");

    for (; (recvBufferDescriptor[currentRecvBuffer].flags & 0x80000000) == 0;
         currentRecvBuffer = (currentRecvBuffer + 1) % 8) {
        if (!(recvBufferDescriptor[currentRecvBuffer].flags & 0x40000000) && (recvBufferDescriptor[currentRecvBuffer].flags & 0x03000000) == 0x03000000)

        {
            uint32_t size = recvBufferDescriptor[currentRecvBuffer].flags & 0xFFF;
            if (size > 64)  // remove checksum
                size -= 4;

            uint8_t *buffer = (uint8_t *)(recvBufferDescriptor[currentRecvBuffer].address);

            if (handler != 0)
                if (handler->OnRawDataReceived(buffer, size))
                    Send(buffer, size);

            size = 64;
            for (int i = 0; i < size; i++) {
                better_os::lib::printhex(buffer[i]);
                printf(" ");
            }
        }
        recvBufferDescriptor[currentRecvBuffer].flags2 = 0;
        recvBufferDescriptor[currentRecvBuffer].flags = 0x8000F7FF;
    }
}

void amd_am79c973::SetHandler(RawDataHandler *handler) {
    this->handler = handler;
}

uint64_t amd_am79c973::GetMACAddress() {
    return initBlock.physicalAddress;
}

void amd_am79c973::SetIPAddress(uint32_t ip) {
    initBlock.logicalAddress = ((uint64_t)ip) << 32;
}

uint32_t amd_am79c973::GetIPAddress() {
    return (uint32_t)(initBlock.logicalAddress >> 32);
}