#ifndef __YUZI_OS__MULTITASKING_H
#define __YUZI_OS__MULTITASKING_H

#include <common/types.h>
#include <gdt.h>

namespace yuzi_os
{
    struct CPUState
    {
        //  EAX, EBX, ECX, EDX, EBP, EDI, and ESI registers are all 32-bit general-purpose registers

        common::uint32_t eax;
        common::uint32_t ebx; // base register
        common::uint32_t ecx; // counting register
        common::uint32_t edx; // data register

        common::uint32_t esi; // stack index
        common::uint32_t edi; // data index
        common::uint32_t ebp; // stack base pointer
        /*
        common::uint32_t gs;
        common::uint32_t fs;
        common::uint32_t es;
        common::uint32_t ds;
        */

        common::uint32_t error; 

        // below are pushed by the processor
        // above are what we need to push (in interruptstabs)
        common::uint32_t eip; // instruction pointer
        common::uint32_t cs;
        common::uint32_t eflags;
        common::uint32_t esp; // stack pointer
        common::uint32_t ss; // stack segment

    } __attribute__((packed));


    class Task
    {
    friend class TaskManager;
    private:
        common::uint8_t stack[4096]; // 4Kib
        CPUState* cpustate;
    public:
        Task(GlobalDescriptorTable *gdt,  void entrypoint()); // entrypoint() as the pointer to the code section (the function)
        ~Task();
    };

    class TaskManager
    {
    private:
        Task* tasks[256];
        int numTasks;
        int currentTask;
    public:
        TaskManager();
        ~TaskManager();
        bool AddTask(Task* task);
        CPUState* Schedule(CPUState* cpustate); // schedule the tasks in a round-robin fashion
    };
}

#endif