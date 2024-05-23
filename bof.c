#include <stdio.h>
#include <unistd.h>

int main(){
        char buf[0x8]={0,};
        read(0,buf,0x100);
        return 0;
}



