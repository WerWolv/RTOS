.section .vtor,"a",%progbits

.syntax unified
.cpu cortex-m7
.thumb

	.word _estack
  	.word entry

  	.word  nmi_handler
  	.word  hard_fault_handler
 	.word  mem_manage_handler
  	.word  bus_fault_handler
  	.word  usage_fault_handler
  	.word  0
  	.word  0
  	.word  0
  	.word  0
  	.word  svc_handler
  	.word  default_handler
  	.word  0
  	.word  pend_sv_handler
  	.word  default_handler
