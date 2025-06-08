#ifndef BETTER_OS_NET_ARP_H
#define BETTER_OS_NET_ARP_H

#include "../lib/types.hpp"
#include "ethernet.hpp"

namespace better_os {
namespace net {
struct AddressResolutionProtocolMessage {
    better_os::lib::uint16_t hardwareType;
    better_os::lib::uint16_t protocol;
    better_os::lib::uint8_t hardwareAddressSize;
    better_os::lib::uint8_t protocolAddressSize;
    better_os::lib::uint16_t command;

    better_os::lib::uint64_t srcMAC : 48;
    better_os::lib::uint32_t srcIP;
    better_os::lib::uint64_t dstMAC : 48;
    better_os::lib::uint32_t dstIP;

} __attribute__((packed));

class AddressResolutionProtocol : public EtherFrameHandler {
    better_os::lib::uint32_t IPcache[128];
    better_os::lib::uint64_t MACcache[128];
    int numCacheEntries;

   public:
    AddressResolutionProtocol(EtherFrameProvider* backend);
    ~AddressResolutionProtocol();

    bool OnEtherFrameReceived(better_os::lib::uint8_t* etherframePayload, better_os::lib::uint32_t size);

    void RequestMACAddress(better_os::lib::uint32_t IP_BE);
    better_os::lib::uint64_t GetMACFromCache(better_os::lib::uint32_t IP_BE);
    better_os::lib::uint64_t Resolve(better_os::lib::uint32_t IP_BE);
};

}  // namespace net
}  // namespace better_os

#endif