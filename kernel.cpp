#include "types.h"
#include "gdt.h"


void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint8_t x = 0, y = 0;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n': // input a linefeed
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];  // preserve the high bytes
                x ++;
                break;
        }

        if (x >= 80) // finish a line 
        {
            x = 0;
            y++;
        }

        if (y >= 25) // finish the whole screen
        {
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

typedef void (*constructor)();       // define constructor as the function pointer
extern "C" constructor start_ctors; // start_ctors as pointer to constructor
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    // it iterate over the space between the start_ctors and end_ctors
    // and jumps to all the function pointers
    for (constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)(); // invoke the function pointer
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t magicnuumber)
{
    printf("Hello World!\n");
    printf("Hello World!");
    
    while (1);
}