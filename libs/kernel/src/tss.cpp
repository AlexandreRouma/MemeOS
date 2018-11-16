#include <libs/kernel/tss.h>
#include <libs/kernel/multiboot.h>
#include <libs/std/string.h>

struct tss kernel_tss;

void initTss() {
    memset(&kernel_tss, 0x00, sizeof(kernel_tss));
    kernel_tss.debug_flag = 0x00;
    kernel_tss.io_map = 0x68;
    kernel_tss.esp0 = (uint32_t)&ASM_STACK_TOP;
    kernel_tss.ss0 = 0x10;
}

void loadTss() {
    asm("  movw $0x20, %ax \n \
            ltr %ax");

    asm("  movw %%ss, %0 \n \
            movl %%esp, %1" : "=m" (kernel_tss.ss0), "=m" (kernel_tss.esp0) : );
}