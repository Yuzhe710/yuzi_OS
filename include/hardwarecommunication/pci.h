
#ifndef __YUZI_OS__HARDWARECOMMUNICATION__PCI_H
#define __YUZI_OS__HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>

namespace yuzi_os
{
    namespace hardwarecommunication
    {

        enum BaseAddressRegisterType
        {
            MemoryMapping = 0,
            InputOutput = 1
        };

        class BaseAddressRegister
        {
        public:
            bool prefetchable;
            yuzi_os::common::uint8_t* address;
            yuzi_os::common::uint32_t size;
            BaseAddressRegisterType type;
        };

        class PeripheralComponentInterconnectDeviceDescriptor
        {
        public:
            yuzi_os::common::uint32_t portBase;
            yuzi_os::common::uint32_t interrupt;

            yuzi_os::common::uint16_t bus;
            yuzi_os::common::uint16_t device;
            yuzi_os::common::uint16_t function;

            yuzi_os::common::uint16_t vendor_id;
            yuzi_os::common::uint16_t device_id;

            yuzi_os::common::uint8_t class_id;
            yuzi_os::common::uint8_t subclass_id;
            yuzi_os::common::uint8_t interface_id;

            yuzi_os::common::uint8_t revision;

            PeripheralComponentInterconnectDeviceDescriptor();
            ~PeripheralComponentInterconnectDeviceDescriptor();

        };

        class PeripheralComponentInterconnectController
        {
            Port32Bit dataPort;
            Port32Bit commandPort;
        
        public: 
            PeripheralComponentInterconnectController();
            ~PeripheralComponentInterconnectController();

            // read a Dword from a PCI configuration register
            yuzi_os::common::uint32_t Read(yuzi_os::common::uint16_t bus, yuzi_os::common::uint16_t device, yuzi_os::common::uint16_t function, yuzi_os::common::uint32_t registeroffset);

            // write a Dword to a PCI configuration register
            void Write(yuzi_os::common::uint16_t bus, yuzi_os::common::uint16_t device, yuzi_os::common::uint16_t function, yuzi_os::common::uint32_t registeroffset, yuzi_os::common::uint32_t value);
            bool DeviceHasFunctions(yuzi_os::common::uint16_t bus, yuzi_os::common::uint16_t device);

            void SelectDrives(yuzi_os::drivers::DriverManager* driverManager, yuzi_os::hardwarecommunication::InterruptManager* interrupts);

            yuzi_os::drivers::Driver* GetDriver(PeripheralComponentInterconnectDeviceDescriptor dev, yuzi_os::hardwarecommunication::InterruptManager* interrupts);
           
            PeripheralComponentInterconnectDeviceDescriptor GetDeviceDescriptor(yuzi_os::common::uint16_t bus, yuzi_os::common::uint16_t device, yuzi_os::common::uint16_t function);

            BaseAddressRegister GetBaseAddressRegister(yuzi_os::common::uint16_t bus, yuzi_os::common::uint16_t device, yuzi_os::common::uint16_t function, yuzi_os::common::uint16_t bar);
        };
    }
}

#endif 