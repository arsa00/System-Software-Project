# .equ init_sp, 0xFFFFFF00
.extern handler

.section my_code_main
ld $0xFFFFFF00, %sp
ld $handler, %r1
csrwr %r1, %handler
halt
wait:
ld read_char, %r1
ld $122, %r2
bne %r1, %r2, wait
halt

.global read_char
.section my_data
read_char:
.word 0
.end