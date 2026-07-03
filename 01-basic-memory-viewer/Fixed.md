### 수정: uint8_t value = *(uint8_t*)currentAddr;에서 uint8_t* value = (uint8_t*)targetAddr;로 수정했습니다.
### 수정: binary 함수에서 targetAddr를 입력받을때 uint8_t 데이터타입에서 LPVOID로 수정. void binary(LPVOID targetAddr).