#include "00_Data_search.h"  // 내가 만든 헤더를 가져옵니다.
#include <stdio.h>
#include <string.h>

// 진짜 전역 변수 메모리 할당 (여기서 공간이 만들어집니다)
uintptr_t found_addresses[MAX_RESULTS];
int found_count = 0;

void append(uintptr_t addr){
    if(found_count < MAX_RESULTS){
        found_addresses[found_count] = addr;
        found_count++;
    }else{
        printf("배열이 가득 찼습니다.\n");
    }
}

void state(LPVOID addr_min, LPVOID addr_max){
    MEMORY_BASIC_INFORMATION mbi;  
    unsigned char* end_addr = (unsigned char*)addr_max;
    unsigned char* start_addr = (unsigned char*)addr_min;

    while(start_addr < end_addr) {
        size_t mbi_result = VirtualQuery(start_addr, &mbi, sizeof(mbi));
        if(mbi_result != sizeof(mbi)){ 
            start_addr += 4096; 
            continue;
        }
        if(mbi.State == MEM_COMMIT && 
          (mbi.Protect & (PAGE_READONLY | PAGE_READWRITE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE))){
            append((uintptr_t)start_addr);
            start_addr += mbi.RegionSize; 
        } else {
            start_addr += mbi.RegionSize; 
        }
    }
}
 
void search(void* target_data, size_t data_size){
    MEMORY_BASIC_INFORMATION mbi;
    int match_count = 0;
    printf("\n[+] 데이터 검색 시작...\n");
    
    for(int i = 0; i < found_count; i++) {
        unsigned char* base_address = (unsigned char*)found_addresses[i];
        if(VirtualQuery(base_address, &mbi, sizeof(mbi)) != sizeof(mbi)) continue;
        unsigned char* region_end = base_address + mbi.RegionSize;

        for(unsigned char* current_ptr = base_address; current_ptr <= region_end - data_size; current_ptr++) {
            if (mbi.Protect & PAGE_GUARD) break;
            if(memcmp(current_ptr, target_data, data_size) == 0) {
                printf("[Found] 주소: 0x%p\n", current_ptr);
                match_count++;
                if(match_count >= 30) return;
            }
        }
    }
}