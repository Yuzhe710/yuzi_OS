#include "types.h"

void printf(char* str)
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;

    for (int i = 0; str[i] != '\0'; ++i)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i]; // preserve the high bytes
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
    printf("Hello World!");
    
    while (1);
}