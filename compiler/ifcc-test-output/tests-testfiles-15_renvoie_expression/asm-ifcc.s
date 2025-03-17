.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $2, %eax
    movl %eax, -4(%rbp)
    movl $3, %eax
    addl -4(%rbp), %eax

    # epilogue
    popq %rbp
    ret
