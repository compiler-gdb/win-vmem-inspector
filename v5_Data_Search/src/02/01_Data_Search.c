#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>

#define MAX_RESULTS 200000
uintptr_t found_addresses[MAX_RESULTS];
int found_count = 0;

void append(uintptr_t addr){
    if(found_count < MAX_RESULTS){
        found_addresses[found_count] = addr;
        addr++;
    }else{
        printf("배열이 가득 찼습니다.\n");
    }
}

void state(LPVOID addr_min, LPVOID addr_max){
    //"하나의 함수는 딱 한 가지 일만 완벽하게 해야 한다"는 것을 반영
    //MEMORY_BASIC_INFORMATION을 인자로 받은 이유는 main에서도  
    
    MEMORY_BASIC_INFORMATION mbi;  

    VirtualQuery(addr_min, &mbi, sizeof(mbi));
    
    unsigned char* end_addr = (unsigned char*)addr_max;
    unsigned char* start_addr = (unsigned char*)addr_min;



    //처음부터 끝까지 돌립니다.
     for(; start_addr < end_addr; start_addr++){

        if(mbi.State == MEM_COMMIT){
            append(start_addr);
        }

     }
}

void search(LPVOID data){

}

int main(void){
    SYSTEM_INFO si; //SYSTEM_INFO는 OS에 정보를 요청하는 시스템 콜. 한 번만 호출해서 인자로 넘기는 것이 효율적.
    GetSystemInfo(&si);

    unsigned char* high_addr = si.lpMaximumApplicationAddress;
    unsigned char* low_addr = si.lpMinimumApplicationAddress;

    return 0;
}