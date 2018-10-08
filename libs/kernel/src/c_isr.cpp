#include <libs/kernel/io.h>
#include <drivers/text_term/terminal.h>
#include <drivers/timer/pit.h>
#include <libs/std/string.h>
#include <drivers/keyboard/keyboard.h>
#include <libs/kernel/panic.h>
#include <drivers/storage/floppy/floppy.h>
#include <libs/kernel/syscalls.h>

#define BochsBreak() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};

char *exception_messages[] ={
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Break Point",
    "Into Detect Overflow",
    "Out of bounds Exception",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS Exception",
    "Segment not present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignement Check Exception",
    "Machine Check Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

extern "C" 
{
    void ISR_0(void) {
        outb(0x20,0x20); // STFU no one cares XD
    }

    void ISR_PIT(void) {
        outb(0x20,0x20);
        PIT.system_ticks++;
    }

    void ISR_KBD(void) {
        Keyboard.update();
        outb(0x20,0x20);
    }

    void ISR_FLOPPY(void) {
        Floppy.irq();
        outb(0x20,0x20);
    }

    void ISR_SYSCALL(uint32_t id, uint32_t ebx, uint32_t ecx, uint32_t edx) {
        KERN_EXEC_SYSCALL(id, ebx, ecx, edx);
    }

    void _fault_handler(struct regs *r) {
        if (r->int_no < 32)
        {
            uint32_t val;
            asm volatile ( "mov %%cr2, %0" : "=r"(val) );
            kernel_panic(val, exception_messages[r->int_no]);
            for (;;);
        }
    }
}

