.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $2, %eax
    movl %eax, -8(%rbp)
    movl $3, %eax
    addl -8(%rbp), %eax
    movl %eax, -12(%rbp)
    movl $4, %eax
    imull -12(%rbp)
    movl %eax, -4(%rbp)
    movl	-4(%rbp), %eax

    # epilogue
    popq %rbp
    ret
