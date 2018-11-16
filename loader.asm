/* Declare constants for the multiboot header. */
.set ALIGN,    1<<0             /* align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* this is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* checksum of above, to prove we are multiboot */

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
.global ASM_STACK_BOTTOM
.global ASM_STACK_TOP
ASM_STACK_BOTTOM:
.skip 16384 # 16 KiB
ASM_STACK_TOP:

.section .text
.global _start
.type _start, @function
_start:
	mov $ASM_STACK_TOP, %esp
	push %ebx
	push %eax
	call kernel_main
	cli

_end:
	hlt
	jmp _end

.size _start, . - _start

.section .sizedetect
.global ASM_KERNEL_END
ASM_KERNEL_END:
	# Kernel size detection
