#include <stdint.h>

class IDT_Class {
    public:
        void load();
};

extern "C" 
{
    extern void ASM_ISR_0(void);
    extern void ASM_ISR_PIT(void);
    extern void ASM_ISR_KBD(void);
    extern void ASM_ISR_FLOPPY(void);
    extern void ASM_ISR_SYSCALL(void);
    extern void _isr0(void);
    extern void _isr1(void);
    extern void _isr2(void);
    extern void _isr3(void);
    extern void _isr4(void);
    extern void _isr5(void);
    extern void _isr6(void);
    extern void _isr7(void);
    extern void _isr8(void);
    extern void _isr9(void);
    extern void _isr10(void);
    extern void _isr11(void);
    extern void _isr12(void);
    extern void _isr13(void);
    extern void _isr14(void);
    extern void _isr15(void);
    extern void _isr16(void);
    extern void _isr17(void);
    extern void _isr18(void);
    extern void _isr19(void);
    extern void _isr20(void);
    extern void _isr21(void);
    extern void _isr22(void);
    extern void _isr23(void);
    extern void _isr24(void);
    extern void _isr25(void);
    extern void _isr26(void);
    extern void _isr27(void);
    extern void _isr28(void);
    extern void _isr29(void);
    extern void _isr30(void);
    extern void _isr31(void);
}

extern IDT_Class IDT;