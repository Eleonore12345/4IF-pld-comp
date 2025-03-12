.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl	$2, -4(%rbp)
    movl	$3, -8(%rbp)
    movl	-4(%rbp), %eax
    movl	%eax, -8(%rbp)
    movl $0, %eax

    # epilogue
    popq %rbp
    ret
