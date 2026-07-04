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

void mem_value(SYSTEM_INFO si){
    MEMORY_BASIC_INFORMATION mbi;

    unsigned char* addr_min = (unsigned char*)si.lpMinimumApplicationAddress;
    unsigned char* addr_max = (unsigned char*)si.lpMaximumApplicationAddress;

    while(addr_min<=addr_max){
        if(VirtualQuery(addr_min, &mbi, sizeof(mbi)) == sizeof(mbi)){
            printf("Address: %p state: %-12s\n", //상태 출력시 첫 글자의 위치가 일관되게 왼쪽 정렬
                addr_min, 
                getStateString(mbi.State));
            addr_min = (unsigned char*)mbi.BaseAddress + mbi.RegionSize;
        }else{
            addr_min += 4096; //윈도우(x86/x64) 시스템에서 메모리를 관리하는 가장 작은 단위의 1페이지의 크기.
                              // VirtyalQuery가 실패했는데 해당 주소에서 다음 주소로 가지 않는다면 무한루프에 걸립니다.
                              // page가 뭐냐? 메모리에 "여기서부터는 읽기 전용이다", "여기는 비었다" 구역을 정하는 최소 단위입니다.
                              // 1byte씩 읽는 것은 책에서 글자 하나하나를 짚어가면서 읽는 것과 같습니다.
                              // 메모리를 관리할 때 사용하는가 데이터를 찾을 때 사용하는가.
        }
    }
}

int main(void){

    SYSTEM_INFO si; // 변수 선언.
    GetSystemInfo(&si); // 시스템 정보를 실제로 si에 채워 넣습니다.

    mem_value(si);

    return 0;
}


/*
MEMORY_BASIC_INFORMATION mbi;
BaseAddress / 영역의 시작 주소 / 현재 보고 있는 블록의 시작 위치.
RegionSize  / 영역의 크기     /  다음 영역으로 건너뛸 때 이만큼 더함.
state       / 메모리 상태     / MEM_COMMIT 상태인지 MEM_FREE 상태인지 확인
Protect     / 접근 권한       / PAGE_READONLY, PAGE_READWRITE 등을 확인.
tYPE        / 메모리 타입     / MEM_IMAGE(이미지 파일), MEM_PRIVATE(개인 데이터) 등을 구분.
*/   