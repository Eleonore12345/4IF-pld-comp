.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $48, %eax

    # epilogue
    popq %rbp
    ret
