#include "all.hpp"

using namespace better_os::lib;
using namespace better_os::basics;

// ---------------------------------------------------
// InterruptHandler Implementation
// ---------------------------------------------------
InterruptHandler::InterruptHandler(InterruptManager* interruptManager, uint8_t interruptNumber) {
    m_interruptNumber = interruptNumber;
    m_interruptManager = interruptManager;
    m_interruptManager->m_handlers[m_interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
    if (m_interruptManager->m_handlers[m_interruptNumber] == this)
        m_interruptManager->m_handlers[m_interruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}

// ---------------------------------------------------
// Static member definitions for InterruptManager
// ---------------------------------------------------
InterruptManager::GateDescriptor InterruptManager::s_interruptDescriptorTable[256];
InterruptManager* InterruptManager::s_activeInterruptManager = 0;

// ---------------------------------------------------
// SetInterruptDescriptorTableEntry Implementation
// ---------------------------------------------------
void InterruptManager::SetInterruptDescriptorTableEntry(
    uint8_t interrupt, uint16_t CodeSegment, void (*handler)(),
    uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType) {
    s_interruptDescriptorTable[interrupt].handlerAddressLowBits =
        ((uint32_t)((uint64_t)handler & 0xFFFFFFFF)) & 0xFFFF;
    s_interruptDescriptorTable[interrupt].handlerAddressHighBits =
        (((uint32_t)((uint64_t)handler & 0xFFFFFFFF)) >> 16) & 0xFFFF;
    s_interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = CodeSegment;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    s_interruptDescriptorTable[interrupt].access =
        IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) |
        DescriptorType;
    s_interruptDescriptorTable[interrupt].reserved = 0;
}

// ---------------------------------------------------
// InterruptManager Implementation
// ---------------------------------------------------
InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable, TaskManager* taskManager)
    : m_programmableInterruptControllerMasterCommandPort(0x20),
      m_programmableInterruptControllerMasterDataPort(0x21),
      m_programmableInterruptControllerSlaveCommandPort(0xA0),
      m_programmableInterruptControllerSlaveDataPort(0xA1) {
    this->m_taskManager = taskManager;
    m_hardwareInterruptOffset = hardwareInterruptOffset;
    uint32_t CodeSegment = globalDescriptorTable->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for (uint8_t i = 255; i > 0; --i) {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
        m_handlers[i] = 0;
    }
    SetInterruptDescriptorTableEntry(0, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    m_handlers[0] = 0;

    SetInterruptDescriptorTableEntry(0x00, CodeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x01, CodeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x02, CodeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x03, CodeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x04, CodeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x05, CodeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x06, CodeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x07, CodeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x08, CodeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x09, CodeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0A, CodeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0B, CodeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0C, CodeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0D, CodeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0E, CodeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x0F, CodeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x10, CodeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x11, CodeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x12, CodeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(0x13, CodeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x02, CodeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x03, CodeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x04, CodeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x05, CodeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x06, CodeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x07, CodeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x08, CodeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x09, CodeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x0A, CodeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x0B, CodeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x0D, CodeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x0E, CodeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(m_hardwareInterruptOffset + 0x0F, CodeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

    SetInterruptDescriptorTableEntry(0x80, CodeSegment, &HandleInterruptRequest0x80, 0, IDT_INTERRUPT_GATE);

    m_programmableInterruptControllerMasterCommandPort.Write(0x11);
    m_programmableInterruptControllerSlaveCommandPort.Write(0x11);

    // Remap the interrupt controllers.
    m_programmableInterruptControllerMasterDataPort.Write(m_hardwareInterruptOffset);
    m_programmableInterruptControllerSlaveDataPort.Write(m_hardwareInterruptOffset + 8);

    m_programmableInterruptControllerMasterDataPort.Write(0x04);
    m_programmableInterruptControllerSlaveDataPort.Write(0x02);

    m_programmableInterruptControllerMasterDataPort.Write(0x01);
    m_programmableInterruptControllerSlaveDataPort.Write(0x01);

    m_programmableInterruptControllerMasterDataPort.Write(0x00);
    m_programmableInterruptControllerSlaveDataPort.Write(0x00);

    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size = 256 * sizeof(GateDescriptor) - 1;
    idt_pointer.base = (uint32_t)((uint64_t)s_interruptDescriptorTable & 0xFFFFFFFF);
    asm volatile("lidt %0" : : "m"(idt_pointer));
}

InterruptManager::~InterruptManager() {
    Deactivate();
}

uint16_t InterruptManager::HardwareInterruptOffset() {
    return m_hardwareInterruptOffset;
}

void InterruptManager::Activate() {
    if (s_activeInterruptManager != 0)
        s_activeInterruptManager->Deactivate();

    s_activeInterruptManager = this;
    asm("sti");
}

void InterruptManager::Deactivate() {
    if (s_activeInterruptManager == this) {
        s_activeInterruptManager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt, uint32_t esp) {
    if (s_activeInterruptManager != 0)
        return s_activeInterruptManager->DoHandleInterrupt(interrupt, esp);
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interrupt, uint32_t esp) {
    if (m_handlers[interrupt] != 0) {
        esp = m_handlers[interrupt]->HandleInterrupt(esp);
    } else if (interrupt != m_hardwareInterruptOffset) {
        printf("UNHANDLED INTERRUPT ");
        printhex(interrupt);
        printf("\n");
    }

    if (interrupt == m_hardwareInterruptOffset) {
        esp = (uint32_t)((uint64_t)m_taskManager->Schedule((CPUState*)esp));
    }

    if (m_hardwareInterruptOffset <= interrupt && interrupt < m_hardwareInterruptOffset + 16) {
        m_programmableInterruptControllerMasterCommandPort.Write(0x20);
        if (m_hardwareInterruptOffset + 8 <= interrupt)
            m_programmableInterruptControllerSlaveCommandPort.Write(0x20);
    }

    return esp;
}
