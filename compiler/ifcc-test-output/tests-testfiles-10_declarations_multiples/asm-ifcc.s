.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl	$2, -0(%rbp)
    movl	$3, -1(%rbp)
    movl	$4, -2(%rbp)
    movl $0, %eax

    # epilogue
    popq %rbp
    ret
