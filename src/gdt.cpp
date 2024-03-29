#include <gdt.h>

using namespace yuzi_os;
using namespace yuzi_os::common;

GlobalDescriptorTable::GlobalDescriptorTable()
:nullSegmentSelector(0,0,0),
unusedSegmentSelector(0,0,0),
codeSegmentSelector(0,64*1024*1024,0x9A), // not too understand the 2nd parameter, 64Mib for the limits
dataSegmentSelector(0,64*1024*1024,0x92)
{
    uint32_t i[2];
    i[1] = (uint32_t)this; // this is a pointer to the gdt, namely the address
    i[0] = sizeof(GlobalDescriptorTable) << 16; // not too sure why shift by 2 bytes

    asm volatile("lgdt (%0)": :"p" (((uint8_t*) i)+2)); // the address of i is denoted by uint8 - load the gdt


}

// desctructor
GlobalDescriptorTable::~GlobalDescriptorTable()
{    
}

uint16_t GlobalDescriptorTable::DataSegmentSelector()
{
    return (uint8_t*)&dataSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::CodeSegmentSelector()
{
    return (uint8_t*)&codeSegmentSelector - (uint8_t*)this;
}

GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t base, uint32_t limit, uint8_t type)
{
    uint8_t* target = (uint8_t*)this; // target is a pointer to the segment entry in GDT
    if (limit <= 65536)
    {
        target[6] = 0x40; // 16-bit address space ?? WHY D/B bit is set?
    }
    else
    {
        // 32 bit address space 
        // Have to squeeze the (32-bit) limit into 2.5 registers (20-bit).
        // This is done by discarding the 12 least significant bits, but this
        // is only legal, if they are all ==1, so they are implicitly still there

        // so if the last bits aren't all 1, we have to set them to 1, but this
        // would increase the limit (cannot do that, because we might go beyond
        // the physical limit or get overlap with other segments) so we have to
        // compensate this by decreasing a higher bit (and might have up to
        // 4095 wasted bytes behind the used memory)

        // Really confusing how that works

        if ((limit & 0xFFF) != 0xFFF)
            limit = (limit >> 12) - 1;
        else
            limit = limit >> 12;
        
        target[6] = 0xC0; // D/B is set for 32-bit segmenting
    }

    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] |= (limit >> 16) & 0xF;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Type
    target[5] = type;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Base()
{
    uint8_t* target = (uint8_t*)this;

    uint32_t result = target[7];
    result = (result << 8) + target[4];
    result = (result << 8) + target[3];
    result = (result << 8) + target[2];

    return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::Limit()
{
    uint8_t* target = (uint8_t*)this;

    uint32_t result = target[6] & 0xF;
    result = (result << 8) + target[1];
    result = (result << 8) + target[0];

    if((target[6] & 0xC0) == 0xC0) // meaning that we are in 32-bit segmentation 
        result = (result << 12) | 0xFFF; // supplement back the 12 bits
    
    return result;

}