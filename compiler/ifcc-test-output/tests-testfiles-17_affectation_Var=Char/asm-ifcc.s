.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $49, %eax
    movl %eax, -4(%rbp)
    movl $50, %eax
    movl %eax, -4(%rbp)
    movl $50, %eax
    movl	-4(%rbp), %eax

    # epilogue
    popq %rbp
    ret
