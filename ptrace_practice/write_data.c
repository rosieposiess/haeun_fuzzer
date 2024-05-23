#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <asm/ptrace.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
        int i, pid;
        int status;
	struct user_regs_struct regs;

        pid=fork();
        if(pid == 0)
        {
                printf("child process worked\n");
                ptrace(PTRACE_TRACEME, 0, 0, 0);
                execve("./target",0, 0); // 실행시키면서 추적상태로 들어감.
        }
	wait(&status);

	ptrace(PTRACE_GETREGS,pid,0,&regs);
	printf("When does target stop?: 0x%llx\n",regs.rip);
	printf("Current stack addr: 0x%llx\n",regs.rsp); 

/*
	for(int j=0;j<1000;j++){
	ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL); // 프로세스가 한 번에 한 단계씩 실행
        wait(&status);
	ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL); // 프로세스가 한 번에 한 단계씩 실행
        wait(&status);
}
*/

	ptrace(PTRACE_GETREGS,pid,0,&regs);
	long long addr=regs.rsp;
	printf("Current instruction address of target: 0x%llx\n",regs.rip);
	printf("Current stack addr: 0x%llx\n",addr);

	addr=0x555555558040;
	for(i=0; i<32; i++)
        {
                if(ptrace(PTRACE_POKEDATA, pid, addr, i) == -1 ) printf("error\n");
		addr += sizeof(int);
        }

	addr=0x555555558040;
/*
	for(i=0;i<32;i++)
	{
		printf("data[%d]:%d\n",i,ptrace(PTRACE_PEEKDATA,pid,addr,0));		
		addr+=sizeof(int);
	}
*/
	ptrace(PTRACE_CONT, pid, 0, 0); //traced 실행재게.

        return 0;
}

