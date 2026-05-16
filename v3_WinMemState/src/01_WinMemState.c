#include <stdio.h>
#include <stdint.h>
#include <windows.h>

void mem_value(SYSTEM_INFO si){
    unsigned char* addr_min = (unsigned char*)si.lpMinimumApplicationAddress;
    unsigned char* addr_max = (unsigned char*)si.lpMaximumApplicationAddress;


    printf("%p\n%p", addr_min, addr_max);

}

int main(void){

    SYSTEM_INFO si;     // 변수 선언.
    GetSystemInfo(&si); // 시스템 정보를 실제로 si에 채워 넣음.

    mem_value(si);

    return 0;
}