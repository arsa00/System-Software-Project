# .equ init_sp, 0xFFFFFF00
.extern handler

.section my_code_main
ld $0xFFFFFF00, %sp
ld $handler, %r7
csrwr %r7, %handler
wait:
ld read_char, %r7
ld $122, %r2
bne %r7, %r2, wait
ld $0x404, %r8
halt

.global read_char
.section my_data
read_char:
.word 0
.end