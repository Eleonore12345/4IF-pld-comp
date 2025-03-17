.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $2, %eax
    movl $eax, -8(%rbp)
    add $eax, -8(%rbp)
    movl %eax, -4(%rbp)

    # epilogue
    popq %rbp
    ret
