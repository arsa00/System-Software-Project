ld $1, %r1
ld $2, %r2
ld $3, %r3
ld $4, %r4
ld $5, %r5
ld $6, %r6
ld $7, %r7
ld $8, %r8
ld $9, %r9
ld $10, %r10
ld $11, %r11
ld $12, %r12
ld $13, %r13
push %r12
pop %r1 # r1 = c
push %r3
div %r3, %r6 # r6 = 2
div %r3, %r5 # r5 = 1
div %r12, %r11 # r11 = 0
halt