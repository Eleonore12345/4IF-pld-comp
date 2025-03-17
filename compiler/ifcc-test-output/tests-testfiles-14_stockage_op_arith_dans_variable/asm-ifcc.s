.globl main
 main: 
    # prologue
    pushq %rbp
    movq %rsp, %rbp

    # body
    movl $2, %eax
    movl %eax, -8(%rbp)
    movl $4, %eax
    movl %eax, -12(%rbp)
    movl $3, %eax
    imull -12(%rbp)
    addl -8(%rbp), %eax
    movl %eax, -16(%rbp)
    movl $1, %eax
    movl %eax, %ecx
    movl -16(%rbp), %eax
    subl %ecx, %eax
    movl %eax, -4(%rbp)
    movl	-4(%rbp), %eax

    # epilogue
    popq %rbp
    ret
