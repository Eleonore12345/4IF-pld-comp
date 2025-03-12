.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl	$2, -4(%rbp)
    movl $0, %eax

    # epilogue
    popq %rbp
    ret
