#include <fcntl.h>
#include <sys/sendfile.h>
#include <seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/prctl.h>
#include <unistd.h>

int main(){
//	int a=0;
	asm(".intel_syntax noprefix");
	asm("mov rax,0x4000000; \
            mov QWORD PTR [rax], 0x1122334455667788");
//	asm("mov WORD PTR [rbp], 0x1122334455667788");
	return 0;
}
