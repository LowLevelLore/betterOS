#include "all.hpp"

using namespace better_os::hardware;
using namespace better_os::lib;

Port::Port(uint16_t portNumber)
    : m_portNumber(portNumber) {
}

Port::~Port() {
}

Port8Bit::Port8Bit(uint16_t portNumber)
    : Port(portNumber) {
}

Port8Bit::~Port8Bit() {
}

void Port8Bit::Write(uint8_t data) {
    Write8(m_portNumber, data);
}

uint8_t Port8Bit::Read() {
    return Read8(m_portNumber);
}

Port8BitSlow::Port8BitSlow(uint16_t portNumber)
    : Port8Bit(portNumber) {
}

Port8BitSlow::~Port8BitSlow() {
}

void Port8BitSlow::Write(uint8_t data) {
    Write8Slow(m_portNumber, data);
}

Port16Bit::Port16Bit(uint16_t portNumber)
    : Port(portNumber) {
}

Port16Bit::~Port16Bit() {
}

void Port16Bit::Write(uint16_t data) {
    Write16(m_portNumber, data);
}

uint16_t Port16Bit::Read() {
    return Read16(m_portNumber);
}

Port32Bit::Port32Bit(uint16_t portNumber)
    : Port(portNumber) {
}

Port32Bit::~Port32Bit() {
}

void Port32Bit::Write(uint32_t data) {
    Write32(m_portNumber, data);
}

uint32_t Port32Bit::Read() {
    return Read32(m_portNumber);
}
