void _start() {
    int x = 8;
    if (x == 8) {
        int y = x * 2;
        x = y;
    }
    asm(
            "push $hello"
            "call puts"
       )
    /* exit system call */
    asm("movl $1,%eax;"
        "xorl %ebx,%ebx;"
        "int  $0x80"
    );
} 
