#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdint.h>


void state(LPVOID addr){
    MEMORY_BASIC_INFORMATION mbi;
    SYSTEM_INFO si;

    unsigned int* start_addr = (int*)addr;

    //처음부터 끝까지 돌린다.
    unsigned int* high_addr = si.lpMaximumApplicationAddress;
    unsigned int* low_addr = si.lpMinimumApplicationAddress;

    for(; low_addr < high_addr; low_addr++){
        //....
    }
}

void search(LPVOID data){

}

int main(void){

    return 0;
}