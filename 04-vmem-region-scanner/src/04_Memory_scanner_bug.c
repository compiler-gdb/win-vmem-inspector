//40번 line에 PAGE_READONLY면 모두 통과시키는 버그 발생

#include <stdio.h>
#include <stdint.h>
#include <windows.h>


const char* GetStateString(DWORD state) {
    switch (state) {
        case MEM_COMMIT:  return "MEM_COMMIT => Start Task";
        case MEM_RESERVE: return "MEM_RESERVE => PASS";
        case MEM_FREE:    return "MEM_FREE => None Data";
        default:          return "UNKNOWN";
    }
}

void print_region_state(unsigned char* addr, MEMORY_BASIC_INFORMATION* mbi){ 
    printf("%p: %-12s", addr, GetStateString(mbi->State)); 
}

void scanning_mem_value(LPVOID targetAdrr, SYSTEM_INFO si){
    
    MEMORY_BASIC_INFORMATION mbi;
    
    unsigned char* addr_max = si.lpMaximumApplicationAddress;
    unsigned char* addr_min = si.lpMinimumApplicationAddress;

    for(; addr_min < addr_max; addr_min+=mbi.RegionSize){
       
        VirtualQuery(addr_min, &mbi, sizeof(mbi));
        
        unsigned char* addr_size = mbi.BaseAddress + mbi.RegionSize;
        unsigned char* base_addr = mbi.BaseAddress;

        printf("\n-------------------------------------------------------------------------------------\n");
        print_region_state(base_addr, &mbi);
        printf("\n-------------------------------------------------------------------------------------\n");
        Sleep(1000);

        if(mbi.State == MEM_COMMIT && VirtualQuery(addr_min, &mbi, sizeof(mbi)) == sizeof(mbi) && mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_READONLY){

            //mbi.baseAddress(영역의 시작 주소)에서 mbi.regionsize 만큼 더하고 <를 이용해서 검사합니다.
            for(base_addr; base_addr < addr_size; base_addr++){
                printf("Address: %p | %02x \n", base_addr, base_addr[0]);
            }
        }
    }
}

int main(void){
    
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    scanning_mem_value(NULL,si);

    return 0;
}