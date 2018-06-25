#include <io.h>
#include <terminal.h>
#include <pit.h>
#include <string.h>
#include <keyboard.h>
#include <panic.h>
#include <syscalls.h>

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

    void ISR_SYSCALL(uint32_t id, uint32_t ebx, uint32_t ecx, uint32_t edx) {
        KERN_EXEC_SYSCALL(id, ebx, ecx, edx);
    }

    void _fault_handler(struct regs *r) {
        if (r->int_no < 32)
        {
            kernel_panic(r->int_no, exception_messages[r->int_no]);
            for (;;);
        }
    }
}

