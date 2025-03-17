.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $0, %eax
    movl %eax, -4(%rbp)

    # epilogue
    popq %rbp
    ret
