#include <stdio.h>
#include <stdint.h> #64비트 시스템 환경에서 주소값을 안전하게 정수형으로 변환해 계산하기 위해 uintptr_t 타입을 사용. uintptr_t 타입은 포인터 크기 정수.
#include <windows.h>
/*
windows.h
BYTE: 1바이트 (unsigned char)
WORD: 2바이트 (unsigned short)
DWORD: 4바이트 (unsigned long)
QWORD: 8바이트 (64비트 정수)

pointer/handle type
LPVOID / PVOID: void*와 동일. "어떤 데이터가 들어있을지 모르는 주소"를 의미, 쉘코드가 들어갈 메모리 할당 시 사용.

HANDLE: 프로세스, 파일 등 시스템 자원을 가리키는 "번호표"입니다. 특정 프로세스에 Attach할 때 사용.

LPCSTR / LPSTR: 문자열 포인터(char*)입니다.
*/

void AnalyzeMemory(LPVOID targetAddr){ //LPVOID: 형태가 정해지지 않은 포인터. 내가 읽어올 데이터가 어떤 자료형일지 모르니 미리 알 수는 없기에 사용.
    uintptr_t currentAddr = (uintptr_t)targetAddr;
    // uintptr_t는 ptr이 붙어있지만 "이 변수에는 메모리 주소로 쓸 수 있을만큼 큰 숫자가 들어있다." 라는걸 알려줄 자료형. 실제 포인터가 아님.
    // 실제로는 integer 즉, 숫자이기 때문에 주소값을 offset 계산 등에 사용하기에 최적화된 자료형. +, -, *, / 같은 산술 연산이 자유로움.
    
    uint64_t value = *(uint64_t*)currentAddr; 
    /*
    1. (uint_64*): 이렇게 형 변환을 하여 숫자에서 Pointer로 격상.
       컴퓨터의 입장에서는 "이 값은 주소인데, 그 주소에 가면 8byte(uint64_t)(64bit)짜리 데이터가 있다"로 이해.
       특징: 단순 위치만이 아닌, 그 위치에 가서 '어디까지(크기)', '어떻게(해석)'읽어야 할지에 대한 정보가 포함."
       어떻게 해석할건지는 int, char, float, void* 등 다양한 방법으로 해석할 수 있다는 뜻.

    2. (uint64_t*)currentAddr: 해당 주소에 "8층짜리 창고가 있다"라는 메모를 붙임. 자료형 변환
    3. *(uint64_t*)currentAddr이 된다면? 해당 메모를 보고 실제로 창고 문을 열고 물건을 꺼내옴.
    */
    
    printf("Address: %p | Value: 0x%llX\n", (void*)currentAddr, value); // %llx: 8byte 정수형인 unigned long long 자료형을 16진수 소문자로 출력하는 서식 지정자.
}

// void binary(LPVOID targetAddr){
//     uintptr_t currentAddr = (uintptr_t)targetAddr;
//     uint64_t value = (uintptr_t*)currentAddr;
//     for (int i=0; i<8; i++){
//         printf("Address: %p", currentAddr[i]);
//     }
// }

void binary(LPVOID targetAddr){
    uint8_t* ptr = (uint8_t*)targetAddr;
    printf("Address: %p | Value: ", targetAddr);

    for (int i = 0; i<8; i++){
        printf("%02x", ptr[i]); // value를 출력해주는 곳. 참조 연산자를 사용한거 같지 않아보이지만 ptr[i] 자체가 *(ptr+1)과 같다. ptr+i 주소로 가서 1byte만큼 꺼내오라는 명령어.
        printf(" ");
    }

    printf("\n");
}

int main(void){
    int test_value = 0x12345678;

    LPVOID targetAddr = &test_value; //LPVOID는 Windows 시스템에서 "어떤 타입인지 모르겠지만 일단 메모리 주소다"라는 것을 의미하는 표준 타입

    AnalyzeMemory(targetAddr);
    binary(targetAddr);

    return 0;
}