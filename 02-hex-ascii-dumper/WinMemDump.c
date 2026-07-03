/*
예시)
Address          00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  ASCII
-------------------------------------------------------------------------
00000005FFE70:   00 00 00 00 78 56 34 12  74 FE 5F 00 00 00 00 00  ....xV4.t._.....
00000005FFE80:   A0 FB 5F 00 00 00 00 00  12 AB 45 00 00 00 00 00  .._.......E.....
*/

#include<stdio.h>
#include<stdint.h>
#include<windows.h>

void mem_data(LPVOID targetAddr){
    uint8_t* addr = (uint8_t*)targetAddr;
    
    printf("%p | ", addr);

    for(int i=0; i<16; i++){
        printf("%02x", addr[i]); //*addr 변수를 받을때 포인터로 받았음. uint8_t라도 주소 자체는 전부 담김.(addr + i)이므로  addr 주소에 있는 data를 꺼내서 1byte 단위로 꺼내옴.
        if (i != 7){
            printf(" ");
        }
        else{
            printf("  ");
        }
    }

    printf(" |  ");

    for(int j=0; j<16; j++){

        if(addr[j] >= 32 && addr[j] <= 126){ // 32~126이 일반적인 문자 범위. 줄바꿈 등의 특수한 경우는 가독성 저하.
            printf("%c", addr[j]); // %c를 통해서 hex 데이터를 char로 변환하여 출력.
        }else{
            printf("."); //깨짐 방지
        }
    }
    printf("\n");
}


int main(void){
    int test_value = 0x12345678;
    LPVOID test_Addr = &test_value;

    printf("     Address     | 00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  |       ASCII\n");

    for (int i=0; i<6; i++){
        mem_data(test_Addr);
        test_Addr = (uint8_t*)test_Addr + 16;
    }

    return 0;
}