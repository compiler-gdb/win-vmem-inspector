#include <stdio.h>
#include <stdint.h>
#include <windows.h>


const char* GetStateString(DWORD state) {
    switch (state) {
        case MEM_COMMIT:  return "MEM_COMMIT";
        case MEM_RESERVE: return "MEM_RESERVE";
        case MEM_FREE:    return "MEM_FREE";
        default:          return "UNKNOWN";
    }
}

void print_region_state(unsigned char* addr, MEMORY_BASIC_INFORMATION* mbi){ 
    //MEMORY_BASIC_INFORMAION* mbi - *가 있는 이유는 호출할 때마다 메모리 구조체 데이터를 계속 들고오는 것이 아니라 데이터의 주소만 알려주게 됩니다.
    //출력하는 역할인데 call by Value를 할 필요는 없습니다.
    printf("%p: %-12s", addr, GetStateString(mbi->State)); 
    //mbi -> State mbi 주소를 따라가서 State의 값을 가져와라. mbi에 주소만 넘겼기에 가능한 규칙입니다.
}

void scanning_mem_value(LPVOID targetAdrr, SYSTEM_INFO si){
    
    MEMORY_BASIC_INFORMATION mbi;
    
    unsigned char* addr_max = si.lpMaximumApplicationAddress;
    unsigned char* addr_min = si.lpMinimumApplicationAddress;

    for(; addr_min < addr_max; addr_min+=mbi.RegionSize){
       
        VirtualQuery(addr_min, &mbi, sizeof(mbi));

        unsigned char* addr_size = mbi.BaseAddress + mbi.RegionSize;
        unsigned char* base_addr = mbi.BaseAddress;

        print_region_state(base_addr, &mbi);
        
        printf("-------------------------------------------------------------------------------------\n");

        if(mbi.State == MEM_COMMIT && VirtualQuery(addr_min, &mbi, sizeof(mbi)) == sizeof(mbi) && mbi.Protect == PAGE_READWRITE || mbi.Protect == PAGE_READONLY){
            
            //mbi.baseAddress(영역의 시작 주소)에서 mbi.regionsize 만큼 더하고 <를 이용해서 검사.
            for(base_addr; base_addr < addr_size; base_addr++){
                printf("Address: %p | %02x \n", base_addr, base_addr[0]);
            }
        }else{
            printf("exceptional\n");
        }
    }
}

int main(void){
    
    SYSTEM_INFO si; // 변수 선언.
    GetSystemInfo(&si);

    scanning_mem_value(NULL,si);

    return 0;
}
