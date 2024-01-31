# .equ init_sp, 0xFFFFFF00
.extern handler

.section my_code_main
ld $0xFFFFFF00, %sp
ld $handler, %r12
csrwr %r12, %handler
ld $0b10, %r12
csrwr %r12, %status
ld $0, %r3

wait:
ld $1000, %r4
beq %r3, %r4, print_msg
ld $1, %r4
add %r4, %r3
ld read_char, %r7
ld $122, %r2
bne %r7, %r2, wait
ld $0x404, %r8
halt

print_msg:
push %r1
ld $msg, %r8
ld $msg, %r1
call printf
pop %r1
halt

printf:
push %r2
push %r3

loop_printf:
ld [%r1], %r2

ld $0x000000FF, %r3
and %r3, %r2 # r2 = r2 & 0x000000ff

beq %r2, %r0, finish_printf
st %r2, 0xFFFFFF00 # term_out

ld $1, %r3
add %r3, %r1 # r1++
jmp loop_printf

finish_printf:
pop %r3
pop %r2
ret

.global read_char
.global msg
.section my_data
read_char:
.word 0

msg:
.ascii "No interrupt occured"
.end