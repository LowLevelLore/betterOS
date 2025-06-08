#ifndef BETTER_OS_NET_ETHERNET_H
#define BETTER_OS_NET_ETHERNET_H

#include "../drivers/amd_am79c973.hpp"
#include "../lib/types.hpp"

namespace better_os {
namespace net {
struct EtherFrameHeader {
    better_os::lib::uint64_t dstMAC_BE : 48;
    better_os::lib::uint64_t srcMAC_BE : 48;
    better_os::lib::uint16_t etherType_BE;
} __attribute__((packed));

typedef better_os::lib::uint32_t EtherFrameFooter;

class EtherFrameProvider;

class EtherFrameHandler {
   protected:
    EtherFrameProvider* backend;
    better_os::lib::uint16_t etherType_BE;

   public:
    EtherFrameHandler(EtherFrameProvider* backend, better_os::lib::uint16_t etherType);
    ~EtherFrameHandler();

    virtual bool OnEtherFrameReceived(better_os::lib::uint8_t* etherframePayload, better_os::lib::uint32_t size);
    void Send(better_os::lib::uint64_t dstMAC_BE, better_os::lib::uint8_t* etherframePayload, better_os::lib::uint32_t size);
};

class EtherFrameProvider : public better_os::drivers::RawDataHandler {
    friend class EtherFrameHandler;

   protected:
    EtherFrameHandler* handlers[65535];

   public:
    EtherFrameProvider(better_os::drivers::amd_am79c973* backend);
    ~EtherFrameProvider();

    bool OnRawDataReceived(better_os::lib::uint8_t* buffer, better_os::lib::uint32_t size);
    void Send(better_os::lib::uint64_t dstMAC_BE, better_os::lib::uint16_t etherType_BE, better_os::lib::uint8_t* buffer, better_os::lib::uint32_t size);

    better_os::lib::uint64_t GetMACAddress();
    better_os::lib::uint32_t GetIPAddress();
};

}  // namespace net
}  // namespace better_os

#endif