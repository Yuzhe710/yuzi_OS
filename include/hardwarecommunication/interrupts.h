
#ifndef __YUZI_OS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H
#define __YUZI_OS__HARDWARECOMMUNICATION__INTERRUPTMANAGER_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <gdt.h>
#include <multitasking.h>

namespace yuzi_os
{
    namespace hardwarecommunication
    {
        class InterruptManager;

        class InterruptHandler
        {
        protected:
            yuzi_os::common::uint8_t interruptNumber; // with a specific interrupt numnber
            InterruptManager* interruptManager;

            InterruptHandler(yuzi_os::common::uint8_t interruptNumber, InterruptManager* interruptManager);
            ~InterruptHandler();
        public:
            virtual yuzi_os::common::uint32_t HandleInterrupt(yuzi_os::common::uint32_t esp);
        };

        class InterruptManager
        {
            // by this friend class, makes it possible for InterruptManager class to access all members (private and public) of InterrruptHandler class as if it were a member of that class . 
            // But InterruptHandler class can't access members of InterruptManager class
            friend class InterruptHandler;

            protected:

                static InterruptManager* ActiveInterruptManager; // a pointer
                InterruptHandler* handlers[256];
                TaskManager* taskManager;

                struct GateDescriptor
                {
                    /* data */
                    yuzi_os::common::uint16_t handlerAddressLowBits;
                    yuzi_os::common::uint16_t gdt_codeSegmentSelector;
                    yuzi_os::common::uint8_t reserved;
                    yuzi_os::common::uint8_t access;
                    yuzi_os::common::uint16_t handlerAddressHighBits;

                } __attribute__((packed));

                static GateDescriptor interruptDescriptorTable[256];

                struct InterruptDescriptorTablePointer
                {
                    yuzi_os::common::uint16_t size;
                    yuzi_os::common::uint32_t base;
                } __attribute__((packed));

                yuzi_os::common::uint16_t hardwareInterruptOffset;

                static void SetInterruptDescriptorTableEntry(
                    yuzi_os::common::uint8_t interruptNumber,
                    yuzi_os::common::uint16_t codeSegmentSelectorOffset,
                    void (*handler)(),
                    yuzi_os::common::uint8_t DescriptorPrivilegeLevel,
                    yuzi_os::common::uint8_t DescriptorType
                );

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

                static yuzi_os::common::uint32_t HandleInterrupt(yuzi_os::common::uint8_t interruptNumber, yuzi_os::common::uint32_t esp); // current stack pointer
                yuzi_os::common::uint32_t DoHandleInterrupt(yuzi_os::common::uint8_t interruptNumber, yuzi_os::common::uint32_t esp);

                Port8BitSlow picMasterCommand;
                Port8BitSlow picMasterData;
                Port8BitSlow picSlaveCommand;
                Port8BitSlow picSlaveData;
            

            public:

                InterruptManager(yuzi_os::common::uint16_t hardwareInterruptOffset, GlobalDescriptorTable* gdt, yuzi_os::TaskManager* taskManager);
                ~InterruptManager();

                yuzi_os::common::uint16_t HardwareInterruptOffset();

                void Activate();
                void Deactivate();

            
        };

    } // namespace hardwarecommunication
    
} // namespace yuzi_os



    
#endif