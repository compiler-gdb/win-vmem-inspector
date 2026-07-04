#include <stdio.h>
#include <stdint.h>
#include <windows.h>

const char* getStateString(DWORD state){
    switch (state){
        case MEM_COMMIT:  return "MEM_COMMIT";
        case MEM_FREE:    return "MEM_FREE";
        case MEM_RESERVE: return "MEM_RESERVE";
        default:          return "UNKNOWN";
    }
}


void mem_value(SYSTEM_INFO* si){
    MEMORY_BASIC_INFORMATION mbi;

    // 탐색할 시작 주소와 끝 주소 설정
    unsigned char* addr_min = (unsigned char*)si->lpMinimumApplicationAddress;
    unsigned char* addr_max = (unsigned char*)si->lpMaximumApplicationAddress;

    // 시작 주소부터 끝 주소까지 탐색
    while(addr_min <= addr_max){
        if(VirtualQuery(addr_min, &mbi, sizeof(mbi)) == sizeof(mbi)){
            // 현재 영역의 State를 출력
            printf("Address: %p | State: %-s\n", (void*)addr_min, getStateString(mbi.State));
            
            addr_min += mbi.RegionSize; //하나의 구역에 State는 모두 동일합니다.
        }else{
            // 만약 특정 주소에서 실패할 경우, 다음 페이지(보통 4KB)로 강제 이동하여 계속 탐색
            addr_min += si->dwPageSize; 
        }
    }
}

int main(void){
    SYSTEM_INFO si;
    GetSystemInfo(&si);

    mem_value(&si);

    return 0;
}