# .equ term_out, 0xFFFFFF00
# .equ term_in, 0xFFFFFF04
# .equ ascii_code, 84 # ascii(’T’)
.extern read_char
.global handler
.section my_code_handler

handler:
push %r1
push %r2
csrrd %cause, %r1
ld $3, %r2
beq %r1, %r2, my_isr_terminal
jmp finish

# obrada prekida od terminala
my_isr_terminal:
ld 0xFFFFFF04, %r1 # term_in
st %r1, 0xFFFFFF00 # term_out
st %r1, read_char

finish:
pop %r2
pop %r1
iret
.end