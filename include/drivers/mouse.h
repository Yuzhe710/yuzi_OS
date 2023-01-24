#ifndef __YUZI_OS__MOUSE_H
#define __YUZI_OS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>

namespace yuzi_os
{
    namespace drivers
    {
        class MouseEventHandler
        {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(yuzi_os::common::uint8_t button);
            virtual void OnMouseUp(yuzi_os::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };

        class MouseDriver : public yuzi_os::hardwarecommunication::InterruptHandler, public Driver
        {
            yuzi_os::hardwarecommunication::Port8Bit dataport;
            yuzi_os::hardwarecommunication::Port8Bit commandport;
            yuzi_os::common::uint8_t buffer[3];
            yuzi_os::common::uint8_t offset;
            yuzi_os::common::uint8_t buttons;

            // int8_t x, y;
            MouseEventHandler* handler;
        public:
            MouseDriver(yuzi_os::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual yuzi_os::common::uint32_t HandleInterrupt(yuzi_os::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif