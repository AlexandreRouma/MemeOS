.org 0x00000000
.global ASM_USER_TASK
.global _END_ASM_USER_TASK

ASM_USER_TASK:
    push %eax
    push %ebx

    mov $0x00, %eax
    mov $msg, %ebx
    int $0x69

    pop %ebx
    pop %eax
    
    ret
_END_ASM_USER_TASK:

msg:
    .ascii "Hello World from userspace task!0"
