/*
* mytraced
*
* Maurice J. Bach 's source.
* "THE DESIGN OF THE UNIX OPERATING SYSTEM"
*/

#include <sys/ptrace.h>
#include <sys/user.h>
#include <asm/ptrace.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() {
	int data[32]={0,};
        int i;
        for( i=0; i<32; i++)
                printf("data[%d] = %d\n", i, data[i]);

        printf("ptrace data addr 0x%x\n", data);
        return 0;
}
