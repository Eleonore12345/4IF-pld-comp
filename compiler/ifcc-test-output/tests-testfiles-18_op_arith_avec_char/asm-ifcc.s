.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $2, %eax
    movl %eax, -12(%rbp)
    movl $48, %eax
    imull -12(%rbp)
    movl %eax, -4(%rbp)
    movl $33, %eax
    movl %eax, -8(%rbp)
    movl	-4(%rbp), %eax
    movl %eax, -16(%rbp)
    movl	-8(%rbp), %eax
    addl -16(%rbp), %eax

    # epilogue
    popq %rbp
    ret
