.set IRQ_BASE, 0x20

.section .text

.extern _ZN7yuzi_os21hardwarecommunication16InterruptManager15HandleInterruptEhj 

.macro HandleException num
.global _ZN7yuzi_os21hardwarecommunication16InterruptManager19HandleException\num\()Ev 
_ZN7yuzi_os21hardwarecommunication16InterruptManager19HandleException\num\()Ev:
    movb $\num, (interruptnumber)
    jmp int_bottom
.endm

.macro HandleInterruptRequest num
.global _ZN7yuzi_os21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev 
_ZN7yuzi_os21hardwarecommunication16InterruptManager26HandleInterruptRequest\num\()Ev:
    movb $\num + IRQ_BASE, (interruptnumber)
    pushl $0       # push 0 for the error variable in CPUstate struct, 0 is filler for the error code For an exception , the processor pushes an error value automatically but for an interrupt we have to push a value ourselves.
    jmp int_bottom
.endm

HandleException 0x00
HandleException 0x01
HandleException 0x02
HandleException 0x03
HandleException 0x04
HandleException 0x05
HandleException 0x06
HandleException 0x07
HandleException 0x08
HandleException 0x09
HandleException 0x0A
HandleException 0x0B
HandleException 0x0C
HandleException 0x0D
HandleException 0x0E
HandleException 0x0F
HandleException 0x10
HandleException 0x11
HandleException 0x12
HandleException 0x13

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01
HandleInterruptRequest 0x02
HandleInterruptRequest 0x03
HandleInterruptRequest 0x04
HandleInterruptRequest 0x05
HandleInterruptRequest 0x06
HandleInterruptRequest 0x07
HandleInterruptRequest 0x08
HandleInterruptRequest 0x09
HandleInterruptRequest 0x0A
HandleInterruptRequest 0x0B
HandleInterruptRequest 0x0C
HandleInterruptRequest 0x0D
HandleInterruptRequest 0x0E
HandleInterruptRequest 0x0F
HandleInterruptRequest 0x31

int_bottom: 

    # save registers
    # pusha
    # pushl %ds
    # pushl %es
    # pushl %fs
    # pushl %gs

    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax

    # call C++ Handler
    pushl %esp
    push (interruptnumber)
    call _ZN7yuzi_os21hardwarecommunication16InterruptManager15HandleInterruptEhj
    # add %esp, 6
    mov %eax, %esp # switch the stack

    # restore registers
    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp
    # popl %gs
    # popl %fs
    # popl %es
    # popl %ds
    # popa

    add $4, %esp

.global _ZN7yuzi_os21hardwarecommunication16InterruptManager15InterruptIgnoreEv
_ZN7yuzi_os21hardwarecommunication16InterruptManager15InterruptIgnoreEv:

    iret

.data
    interruptnumber: .byte 0

