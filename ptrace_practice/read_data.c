#include <sys/ptrace.h>
#include <sys/user.h>
#include <asm/ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	struct user_regs_struct regs;
	unsigned int data;
	unsigned char buff[4];
	int ret, pid, i, j;
	int status;

	pid=fork();

	if(pid==0){
		ptrace(PTRACE_TRACEME,0,0,0);
		execve("./target",0,0);}

	wait(&status);

	ptrace(PTRACE_GETREGS, pid, 0, &regs);
	printf("stack = 0x%p\n", regs.rsp);

	for (i=0; i<10; i++) {
		data = ptrace(PTRACE_PEEKDATA, pid, regs.rsp+i*4, 0);
		memcpy(buff, &data, 4);
		printf("0x%p : ",regs.rsp+i*4);
		for (j=0; j<4; j++) {
			if (isprint(buff[j]))
				printf("%c ", buff[j]);
			else
				printf(". ");
		}
		printf("%08x\n", data);
	}

	//write
	ptrace(PTRACE_POKEDATA, pid, regs.rsp, 0xa5a5a5a5);
	printf("data: 0x%x",ptrace(PTRACE_PEEKDATA,pid,regs.rsp,0));
	ptrace(PTRACE_DETACH, pid, 0, 0);

	return 0;
}
