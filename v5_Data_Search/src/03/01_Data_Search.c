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

void state(LPVOID addr_min, LPVOID addr_max){ //COMMIT 상태인 Region의 시작 주소를 알려줍니다.
    
    MEMORY_BASIC_INFORMATION mbi;  
    
    unsigned char* end_addr = (unsigned char*)addr_max;
    unsigned char* start_addr = (unsigned char*)addr_min;


     for(; start_addr < end_addr; start_addr += mbi.RegionSize){

        size_t mbi_result = VirtualQuery(start_addr, &mbi, sizeof(mbi)); // VirtualQuery를 1번만 호출하여 비교하기 위함입니다.

        if(mbi_result != sizeof(mbi)){ //운영체제 내부 공간이나 존재하지 않는 주소라면?
            start_addr += 4096;
            continue;
            //메모리를 관리할 때는 딱 4096바이트(4KB)씩 블록으로 묶어서 관리합니다. 
            //이 한 블록을 페이지(Page)라고 부릅니다.
        }

        else if(VirtualQuery(addr_min, &mbi, sizeof(mbi)) == sizeof(mbi) && mbi.State == MEM_COMMIT && 
        (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))){
            //if (*(int*)current_ptr == target_value) //만약 int형을 읽을 경우 다른 주소의 침범을 막기 위해서는-4를 해야함.
            append(start_addr);
            continue;
        }

        else{
            printf("Error\n");
        }
     }
}
 

void search(LPVOID addr){
    unsigned char* commit_addr = addr;
    
    
    //found_addresses[i]
}

int main(void){
    SYSTEM_INFO si; //SYSTEM_INFO는 OS에 정보를 요청하는 시스템 콜. 한 번만 호출해서 인자로 넘기는 것이 효율적입니다.
    GetSystemInfo(&si);

    unsigned char* high_addr = si.lpMaximumApplicationAddress;
    unsigned char* low_addr = si.lpMinimumApplicationAddress;

    return 0;
}