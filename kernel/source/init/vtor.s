.section .vtor,"a",%progbits

.syntax unified
.cpu cortex-m7
.thumb

.word _estack
.word entry

  .word  default_handler
  .word  default_handler
  .word  default_handler
  .word  default_handler
  .word  default_handler
  .word  0
  .word  0
  .word  0
  .word  0
  .word  default_handler
  .word  default_handler
  .word  0
  .word  default_handler
  .word  default_handler
