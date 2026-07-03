#include <stdio.h>
#include "00_Data_search.h"

int main(void){
    SYSTEM_INFO si; 
    GetSystemInfo(&si);

    unsigned char* low_addr = si.lpMinimumApplicationAddress;
    unsigned char* high_addr = si.lpMaximumApplicationAddress;

    printf("[+] 스캔 시작 범위: 0x%p ~ 0x%p\n", low_addr, high_addr);

    state(low_addr, high_addr);
    printf("[+] 수집된 유효 Region 개수: %d\n", found_count);

    
    int target_int = 12345678; 
    printf("\n--> [테스트 1] 정수 %d 검색 시도", target_int);
    search(&target_int, sizeof(target_int));

    char target_str[] = "hello";
    printf("\n--> [테스트 2] 문자열 \"%s\" 검색 시도", target_str);
    search(target_str, strlen(target_str));

    return 0;
}