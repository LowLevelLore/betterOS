#ifndef __BETTER_OS_INTERRUPTS_H
#define __BETTER_OS_INTERRUPTS_H

#include "../lib/types.hpp"
#include "gdt.hpp"
#include "../hardware/port.hpp"

namespace better_os
{
    namespace basics
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            better_os::lib::uint8_t InterruptNumber;
            InterruptManager *interruptManager;
            InterruptHandler(InterruptManager *interruptManager, better_os::lib::uint8_t InterruptNumber);
            ~InterruptHandler();

        public:
            virtual better_os::lib::uint32_t HandleInterrupt(better_os::lib::uint32_t esp);
        };

        class InterruptManager
        {
            friend class InterruptHandler;

        protected:
            static InterruptManager *ActiveInterruptManager;
            InterruptHandler *handlers[256];

            struct GateDescriptor
            {
                better_os::lib::uint16_t handlerAddressLowBits;
                better_os::lib::uint16_t gdt_codeSegmentSelector;
                better_os::lib::uint8_t reserved;
                better_os::lib::uint8_t access;
                better_os::lib::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                better_os::lib::uint16_t size;
                better_os::lib::uint32_t base;
            } __attribute__((packed));

            better_os::lib::uint16_t hardwareInterruptOffset;
            static void SetInterruptDescriptorTableEntry(better_os::lib::uint8_t interrupt,
                                                         better_os::lib::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                                                         better_os::lib::uint8_t DescriptorPrivilegeLevel, better_os::lib::uint8_t DescriptorType);

            static void InterruptIgnore();

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            static better_os::lib::uint32_t HandleInterrupt(better_os::lib::uint8_t interrupt, better_os::lib::uint32_t esp);
            better_os::lib::uint32_t DoHandleInterrupt(better_os::lib::uint8_t interrupt, better_os::lib::uint32_t esp);

            better_os::hardware::Port8BitSlow programmableInterruptControllerMasterCommandPort;
            better_os::hardware::Port8BitSlow programmableInterruptControllerMasterDataPort;
            better_os::hardware::Port8BitSlow programmableInterruptControllerSlaveCommandPort;
            better_os::hardware::Port8BitSlow programmableInterruptControllerSlaveDataPort;

        public:
            InterruptManager(better_os::lib::uint16_t hardwareInterruptOffset, GlobalDescriptorTable *globalDescriptorTable);
            ~InterruptManager();
            better_os::lib::uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();
        };
    }
}

#endif // !__BETTER_OS_INTERRUPTS_H