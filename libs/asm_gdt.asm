.global ASM_RELOAD_SEGMENTS

ASM_RELOAD_SEGMENTS:
    mov 0x10, %ax
    mov %ds, %ax
    mov %es, %ax
    mov %fs, %ax
    mov %gs, %ax
    mov %ss, %ax
    ljmp $0x08, $flush2

flush2:
    ret
