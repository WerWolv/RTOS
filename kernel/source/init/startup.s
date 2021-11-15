.section .text.startup

.syntax unified
.cpu cortex-m7
.thumb

.global entry

entry:
    /* Setup stack pointer */
    ldr sp, =_estack


    /* Copy .data segment */
    ldr r0, =_sdata
    ldr r1, =_edata
    ldr r2, =_sidata
    movs r3, #0

    b copy_data_inc

    copy_data_copy:
        ldr r4, [r2, r3]
        str r4, [r0, r3]
        adds r3, r3, #4

    copy_data_inc:
        adds r4, r0, r3
        cmp r4, r1
        bcc copy_data_copy

    /* Zero out .bss segment */

    ldr r2, =_sbss
    ldr r4, =_ebss
    movs r3, #0
    b fill_bss_check

    fill_bss_set:
        str  r3, [r2]
        adds r2, r2, #4

    fill_bss_check:
        cmp r2, r4
        bcc fill_bss_set

    bl kern_main
    bx lr