#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::drivers;

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(
    bool master, better_os::lib::uint16_t portBase)
    : dataPort(portBase), errorPort(portBase + 0x1), sectorCountPort(portBase + 0x2), lbaLowPort(portBase + 0x3), lbaMidPort(portBase + 0x4), lbaHighPort(portBase + 0x5), devicePort(portBase + 0x6), commandPort(portBase + 0x7), controlPort(portBase + 0x206), master(master) {}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment() {}

void *AdvancedTechnologyAttachment::Identify() {
    devicePort.Write(master ? 0xA0 : 0xB0);
    controlPort.Write(0);

    devicePort.Write(0xA0);
    uint8_t status = commandPort.Read();
    if (status == 0xFF)
        return nullptr;

    devicePort.Write(master ? 0xA0 : 0xB0);
    sectorCountPort.Write(0);
    lbaLowPort.Write(0);
    lbaMidPort.Write(0);
    lbaHighPort.Write(0);
    commandPort.Write(0xEC);  // IDENTIFY command

    status = commandPort.Read();
    if (status == 0x00)
        return nullptr;

    while ((status & 0x80) && !(status & 0x01))
        status = commandPort.Read();

    if (status & 0x01) {
        printf("ERROR in IDENTIFY\n");
        return nullptr;
    }

    // Allocate 513 bytes: 512 for data + 1 for null terminator
    char *identityData = new char[513];
    if (!identityData)
        return nullptr;

    int index = 0;
    for (int i = 0; i < 256; ++i) {
        uint16_t data = dataPort.Read();
        if (index + 1 < 512) {
            identityData[index++] = (data >> 8) & 0xFF;  // High byte
            identityData[index++] = data & 0xFF;         // Low byte
        }
    }

    identityData[512] = '\0';  // Null-terminate the string
    return identityData;
}

char *AdvancedTechnologyAttachment::Read28(uint32_t sectorNum, int count) {
    if (sectorNum > 0x0FFFFFFF || count <= 0 || count > 512)
        return nullptr;

    // Allocate buffer using custom MemoryManager
    char *buffer = new char[count + 1];  // +1 for null terminator
    if (!buffer)
        return nullptr;

    devicePort.Write((master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(1);
    lbaLowPort.Write(sectorNum & 0x000000FF);
    lbaMidPort.Write((sectorNum & 0x0000FF00) >> 8);
    lbaHighPort.Write((sectorNum & 0x00FF0000) >> 16);
    commandPort.Write(0x20);  // READ command

    // Wait for DRQ and check for errors
    uint8_t status = commandPort.Read();
    while (((status & 0x80) == 0x80) && !(status & 0x08) && !(status & 0x01))
        status = commandPort.Read();

    if (status & 0x01) {
        printf("ERROR: Read28 failed\n");
        delete[] buffer;
        return nullptr;
    }

    // Read data word by word (16-bit)
    int i = 0;
    for (; i + 1 < count; i += 2) {
        uint16_t wdata = dataPort.Read();
        buffer[i] = wdata & 0xFF;
        buffer[i + 1] = (wdata >> 8) & 0xFF;
    }

    if (i < count) {
        uint16_t wdata = dataPort.Read();
        buffer[i] = wdata & 0xFF;
        ++i;
    }

    // Discard unused words from rest of 512-byte sector
    for (; i < 512; i += 2)
        dataPort.Read();

    buffer[count] = '\0';  // Null-terminate
    return buffer;
}

void AdvancedTechnologyAttachment::Write28(uint32_t sectorNum, uint8_t *data,
                                           uint32_t count) {
    if (sectorNum > 0x0FFFFFFF)
        return;
    if (count > 512)
        return;

    devicePort.Write((master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24));
    errorPort.Write(0);
    sectorCountPort.Write(1);
    lbaLowPort.Write(sectorNum & 0x000000FF);
    lbaMidPort.Write((sectorNum & 0x0000FF00) >> 8);
    lbaLowPort.Write((sectorNum & 0x00FF0000) >> 16);
    commandPort.Write(0x30);

    for (int i = 0; i < count; i += 2) {
        uint16_t wdata = data[i];
        if (i + 1 < count)
            wdata |= ((uint16_t)data[i + 1]) << 8;
        dataPort.Write(wdata);

        char *text = "  \0";
        text[0] = (wdata >> 8) & 0xFF;
        text[1] = wdata & 0xFF;
    }

    for (int i = count + (count % 2); i < 512; i += 2)
        dataPort.Write(0x0000);
}

void AdvancedTechnologyAttachment::Flush() {
    devicePort.Write(master ? 0xE0 : 0xF0);
    commandPort.Write(0xE7);

    uint8_t status = commandPort.Read();
    if (status == 0x00)
        return;

    while (((status & 0x80) == 0x80) && ((status & 0x01) != 0x01))
        status = commandPort.Read();

    if (status & 0x01) {
        printf("ERROR");
        return;
    }
}