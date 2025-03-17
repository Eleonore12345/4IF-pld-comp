.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $2, %eax
    movl %eax, -4(%rbp)
    movl $3, %eax
    movl %eax, -8(%rbp)
    movl	-4(%rbp), %eax
    movl %eax, -8(%rbp)
    movl	-4(%rbp), %eax
    movl	-8(%rbp), %eax

    # epilogue
    popq %rbp
    ret
