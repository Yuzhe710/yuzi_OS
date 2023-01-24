#ifndef __YUZI_OS_HARDWARECOMMUNICATION__PORT_H
#define __YUZI_OS_HARDWARECOMMUNICATION__PORT_H


#include <common/types.h>

namespace yuzi_os
{
    namespace hardwarecommunication
    {
        class Port
        {
            protected:
                Port(yuzi_os::common::uint16_t portnumber);
                ~Port();
                yuzi_os::common::uint16_t portnumber;
        };

        class Port8Bit : public Port
        {
            public:
                Port8Bit(yuzi_os::common::uint16_t portnumber);
                ~Port8Bit();

                virtual yuzi_os::common::uint8_t Read();
                virtual void Write(yuzi_os::common::uint8_t data);
    
            protected:
                static inline yuzi_os::common::uint8_t Read8(yuzi_os::common::uint16_t _port) 
                {
                    yuzi_os::common::uint8_t result;
                    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
                    return result;
                }

                static inline void Write8(yuzi_os::common::uint16_t _port, yuzi_os::common::uint8_t _data)
                {
                    __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
                }
        };

        

        class Port8BitSlow : public Port8Bit
        {
            public:
                Port8BitSlow(yuzi_os::common::uint16_t portnumber);
                ~Port8BitSlow();

                virtual void Write(yuzi_os::common::uint8_t data);
            protected:
                static inline void Write8Slow(yuzi_os::common::uint16_t _port, yuzi_os::common::uint8_t _data)
                {
                    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
                }
        };

        class Port16Bit : public Port
        {
            public:
                Port16Bit(yuzi_os::common::uint16_t portnumber);
                ~Port16Bit();

                virtual yuzi_os::common::uint16_t Read();
                virtual void Write(yuzi_os::common::uint16_t data);

            protected:
                static inline yuzi_os::common::uint16_t Read16(yuzi_os::common::uint16_t _port)
                {
                    yuzi_os::common::uint16_t result;
                    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
                    return result;
                }

                static inline void Write16(yuzi_os::common::uint16_t _port, yuzi_os::common::uint16_t _data)
                {
                    __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
                }
        };



        class Port32Bit : public Port
        {
            public:
                Port32Bit(yuzi_os::common::uint16_t portnumber);
                ~Port32Bit();

                virtual yuzi_os::common::uint32_t Read();
                virtual void Write(yuzi_os::common::uint32_t data);

            protected:
                static inline yuzi_os::common::uint32_t Read32(yuzi_os::common::uint16_t _port)
                {
                    yuzi_os::common::uint32_t result;
                    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
                    return result;
                }

                static inline void Write32(yuzi_os::common::uint16_t _port, yuzi_os::common::uint32_t _data)
                {
                    __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
                }
        };
    }
}

    




#endif