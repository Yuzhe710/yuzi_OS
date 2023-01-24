#ifndef __YUZIOS__COMMON__TYPES_H
#define __YUZIOS__COMMON__TYPES_H

namespace yuzi_os
{
    namespace common
    {
        typedef char int8_t;
        typedef unsigned char uint8_t;

        typedef short int16_t;
        typedef unsigned short uint16_t;

        typedef int int32_t;
        typedef unsigned int uint32_t;

        typedef long long int int64_t;
        typedef unsigned long long int uint64_t;
    } // namespace common
} // namespace yuzi_os


// Make the type byte precise. Very important when communicating with hardware

#endif