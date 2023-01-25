#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/driver.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>

using namespace yuzi_os;
using namespace yuzi_os::common;
using namespace yuzi_os::drivers;
using namespace yuzi_os::hardwarecommunication;

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

void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

class PrintfKeyboardEventHandler : public KeyboardEventHandler
{
public:

    void OnKeyDown(char c)
    {
        char* foo = " ";
        foo[0] = c;
        printf(foo);
    }
};


class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:

    MouseToConsole()
    {
    }

    virtual void OnActivate()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }

    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }

};

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
    
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);

    // Activate hardware
    printf("Initializing Hardware, Stage 1\n");
    DriverManager drvManager;


    //KeyboardDriver keyboard(&interrupts);
    //drvManager.AddDriver(&keyboard);
    //MouseDriver mouse(&interrupts);
    //drvManager.AddDriver(&mouse);

    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);

    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    drvManager.AddDriver(&mouse);

    PeripheralComponentInterconnectController PCIController;
    PCIController.SelectDrives(&drvManager);

    printf("Initializing Hardware, Stage 2\n");
        drvManager.ActivateAll();

    printf("Initializing Hardware, Stage 3\n");
    interrupts.Activate();

    // InterruptManager interrupts(0x20, &gdt);
    // KeyboardDriver keyboard(&interrupts);
    // interrupts.Activate();

    while (1);
}