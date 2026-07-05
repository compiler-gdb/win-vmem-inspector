// PAGE_READONLY이면 STATE 상관없이 모두 출력하는 버그를 수정했습니다.

#include <stdio.h>
#include <stdint.h>
#include <windows.h>

const char* GetStateString(DWORD state) {
    switch (state) {
        case PAGE_READWRITE:  return "PAGE_READWRITE";
        case PAGE_READONLY:   return "PAGE_READONLY";
        default:              return "UNKNOWN";
    }
}


void scanning_mem_value(LPVOID targetAdrr, SYSTEM_INFO si){
    
    MEMORY_BASIC_INFORMATION mbi;
    
    unsigned char* addr_max = si.lpMaximumApplicationAddress;
    unsigned char* addr_min = si.lpMinimumApplicationAddress;

    for(; addr_min < addr_max; addr_min+=mbi.RegionSize){
       
        VirtualQuery(addr_min, &mbi, sizeof(mbi));
        
        unsigned char* addr_size = mbi.BaseAddress + mbi.RegionSize;
        unsigned char* base_addr = mbi.BaseAddress;



        if(mbi.State == MEM_COMMIT && VirtualQuery(addr_min, &mbi, sizeof(mbi)) == sizeof(mbi) && (mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_READONLY)){
            printf("\n-------------------------------------------------------------------------------------\n%p: COMMIT", mbi.BaseAddress);
            printf("/%s\n-------------------------------------------------------------------------------------\n", GetStateString(mbi.Protect));
            Sleep(1000);
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

