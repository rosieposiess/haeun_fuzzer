#include <sys/ptrace.h>
#include <sys/user.h>
#include <asm/ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int data[32]={0,};
int main() {
        int i;
	printf("I'm target\n");
        for( i=0; i<32; i++)
                printf("data[%d] = %d\n", i, data[i]);

        printf("ptrace data addr 0x%llx\n", data);
        return 0;
}
