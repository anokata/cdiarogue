#include <stdio.h>

int main() {
    char* hello = "Hello asm in C";
    asm(
            "push $hello;"
            "call puts;"
            "add $4, %esp;"
       );
    asm("movl $1,%eax;"
        "xorl %ebx,%ebx;"
        "int  $0x80"
    );
} 
