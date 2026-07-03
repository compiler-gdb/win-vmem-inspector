#pragma once
#include <windows.h>
#include <stdint.h>

#define MAX_RESULTS 200000

// [핵심] extern을 붙이면 "이 변수의 실제 메모리는 다른 .c 파일에 있으니 이름만 참고해라"라는 뜻이 됩니다.
extern uintptr_t found_addresses[MAX_RESULTS];
extern int found_count;

// 함수의 모양(선언)만 적어둡니다.
void append(uintptr_t addr);
void state(LPVOID addr_min, LPVOID addr_max);
void search(void* target_data, size_t data_size);