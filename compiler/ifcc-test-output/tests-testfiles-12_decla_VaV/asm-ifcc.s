.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl	$2, -0(%rbp)
    movl	-0(%rbp), %eax
    movl	%eax, -0(%rbp)
    movl $0, %eax

    # epilogue
    popq %rbp
    ret
