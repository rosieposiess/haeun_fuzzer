#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <asm/ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
        struct user_regs_struct regs;
        int ret, status;

        pid_t pid = fork();
        if (pid == 0) {
                // child process
                ptrace(PTRACE_TRACEME, 0, 0, 0);
                execve("./target",0,0);
        }

	//parent
        wait(&status);
        if (WIFSIGNALED(status)) {
                printf("child process %d was abnormal exit.\n", pid);
                return -1;
        }

        ret = ptrace(PTRACE_GETREGS, pid,0,&regs);
        printf("return : %d\n", ret);
        printf("stack = %p\n",regs.rsp);
        printf("pc    = %p\n",regs.rip);

        ptrace(PTRACE_KILL, pid, 0, 0);

        return 0;
}
