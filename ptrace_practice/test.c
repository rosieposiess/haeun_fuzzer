#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/user.h>

int main() {
    pid_t child;
    long orig_eax;
    int status,ret;
    struct user_regs_struct regs;

    child = fork();
    if(child == 0) {
        // 자식 프로세스에서 실행될 코드
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execl("./target", "./target", NULL);
    } else {
        // 부모 프로세스에서 실행될 코드
        wait(&status);
        while(WIFSTOPPED(status)) { // WIFSTOPPED(status)는 정지 상태인 경우 true, 그렇지 않은 경우 false
            ret=ptrace(PTRACE_GETREGS, child, NULL, &regs);
            printf("Return val:%d\n",ret);
            printf("RIP: %llx\n", regs.rip); // x86_64 아키텍처에서의 RIP
	    printf("RSP: %llx\n",regs.rsp);
            ptrace(PTRACE_SINGLESTEP, child, NULL, NULL); // 프로세스가 한 번에 한 단계씩 실행
            wait(&status);
        }
    }
    return 0;
}
