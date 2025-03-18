.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $1, %eax
    movl %eax, -4(%rbp)
    movl $1, %eax
    movl $2, %eax
    movl %eax, -16(%rbp)
    movl $4, %eax
    imull -16(%rbp)
    movl %eax, -20(%rbp)
    movl $5, %eax
    imull -20(%rbp)
    movl %eax, -8(%rbp)
    movl $2, %eax
    movl %eax, -24(%rbp)
    movl $4, %eax
    imull -24(%rbp)
    movl %eax, -28(%rbp)
    movl $5, %eax
    imull -28(%rbp)
    movl	-4(%rbp), %eax
    movl %eax, -32(%rbp)
    movl	-8(%rbp), %eax
    addl -32(%rbp), %eax
    movl %eax, -12(%rbp)
    movl	-12(%rbp), %eax

    # epilogue
    popq %rbp
    ret
