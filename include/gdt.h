#ifndef __YUZI_OS__GDT_H
#define __YUZI_OS__GDT_H

#include <common/types.h>

namespace yuzi_os
{
    class GlobalDescriptorTable
    {

        public:
            class SegmentDescriptor
            {
            private:
                yuzi_os::common::uint16_t limit_lo;
                yuzi_os::common::uint16_t base_lo;
                yuzi_os::common::uint8_t base_hi;
                yuzi_os::common::uint8_t type;
                yuzi_os::common::uint8_t flags_limit_hi;
                yuzi_os::common::uint8_t base_vhi;
            public:
                SegmentDescriptor(yuzi_os::common::uint32_t base, yuzi_os::common::uint32_t limit, yuzi_os::common::uint8_t type);
                yuzi_os::common::uint32_t Base();
                yuzi_os::common::uint32_t Limit(); 

            } __attribute__((packed));


        private:
          SegmentDescriptor nullSegmentSelector;
          SegmentDescriptor unusedSegmentSelector;
          SegmentDescriptor codeSegmentSelector;
          SegmentDescriptor dataSegmentSelector;
    
        public:
            GlobalDescriptorTable();
            ~GlobalDescriptorTable(); // desctructor

            yuzi_os::common::uint16_t CodeSegmentSelector();
            yuzi_os::common::uint16_t DataSegmentSelector();
    };

}

    
#endif