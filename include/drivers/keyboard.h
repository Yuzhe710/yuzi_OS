
#ifndef __YUZI_OS__DRIVERS__KEYBOARD_H
#define __YUZI_OS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/driver.h>

namespace yuzi_os
{
    namespace drivers
    {
        // it is a base class for keyboard handlers from which various keyboard
        // handlers classes will be derived.
        class KeyboardEventHandler
        {
        public:
            KeyboardEventHandler();

            virtual void OnKeyDown(char);
            virtual void OnKeyUp(char);
        };

        class KeyboardDriver : public yuzi_os::hardwarecommunication::InterruptHandler, public Driver
        {
            yuzi_os::hardwarecommunication::Port8Bit dataport;
            yuzi_os::hardwarecommunication::Port8Bit commandport;

            KeyboardEventHandler* handler;
        public:
            KeyboardDriver(yuzi_os::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler* handler);
            ~KeyboardDriver();
            virtual yuzi_os::common::uint32_t HandleInterrupt(yuzi_os::common::uint32_t esp);
            virtual void Activate();
        };

    }
}
    

#endif