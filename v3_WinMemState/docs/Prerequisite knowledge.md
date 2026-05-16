2026_05_14 01:50

unsigned char* addr_min = (unsigned char*)si.lpMinimumApplicationAddress;
unsigned char* addr_max = (unsigned char*)si.lpMaximumApplicationAddress;

unsigned가 뭐냐. -128~127 같이 절반은 음수, 절반은 양수로 사용하는게 아닌 0~255같이 0과 양수만 사용.
메모리 주소는 음수가 없다.
    
char를 사용하는가? -> unsigned가 붙으면 일단 숫자로 인식, *로 인해서 주소로 받아들인다. char는 1byte 크기의 자료형. 1byte씩 키워가면서 주소를 살펴보고자 하는 의도가 있음. int였다면 4byte씩 커졌을 것.
    

(unsigned char*)si.lpMinimumApplicationAddress

1. si.lpMinimumApplicationAddress를 하면 컴퓨터 입장에서는 큰 숫자일 뿐이다.
2. (unsigned char*)

    2-1. * -> 이 큰 숫자는 데이터가 아닌 어딘가를 가르키는 주소다. 라고 알림.
    2-2. unsigned char: 그 주소에 가면 1byte짜리 칸들이 나열되고, 해당 주소에 도착했다면 나는 그것을 0~255 사이의 정수로 읽을 준비가 되었다.
    


addr_min은 어떻게 이 데이터를 전달받는가?

3. addr_min이라는 이름의 저장 공간은 8bite(x64기준) 이 공간에는 오직 "숫자만" 저장됩니다.

    3-1 * -> addr_min 안에는 큰 숫자가 있는데 *가 있으니 이건 숫자가 아닌 어딘가의 좌표라고 인식합니다.
    3-2 unsigned char: 만약 좌표에 도착했다면 딱 1byte 데이터만 들여다 보고, 그 안의 데이터는 0~255 사이의 숫자로만 해석합니다.*/


주의: 포인터는 "도착하고 나서 데이터를 읽는다면" 1byte씩 읽는거지, addr_min 변수에 바로 데이터가 들어가는 것은 아닙니다.
addr_min, addr_max는 포인터 변수이기 때문에 변수의 값은 항상 "주소"입니다.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

main()에_SYSTEM INFO가 뭐냐?

typedef struct _SYSTEM_INFO {
  union {
    DWORD dwOemId;          // 예전 방식의 ID
    struct {
      WORD wProcessorArchitecture; // 프로세서 아키텍처 (x64 등)
      WORD wReserved;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  DWORD     dwPageSize;      // 페이지 크기
  LPVOID    lpMinimumApplicationAddress; // 최소 주소
  LPVOID    lpMaximumApplicationAddress; // 최대 주소
  DWORD_PTR dwActiveProcessorMask;
  DWORD     dwNumberOfProcessors;
  DWORD     dwProcessorType;
  DWORD     dwAllocationGranularity;
  WORD      wProcessorLevel;
  WORD      wProcessorRevision;
} SYSTEM_INFO, *PSYSTEM_INFO; 

맨 밑줄의 SYSTEM_INFO는 이 구조체의 별명. 그래서 우리는 별명을 통해 SYSTEM_INFO si 같이 간단히 변수 생성 가능합니다.
맨 밑줄의 PSYSTEM_INFO는 이 구제체의 포인터형 별명. SYSTEM_INFO*와 동일합니다.


----------------------------------------------------------------------------------------------------------------------------------------------------------


2026_05_14 14:25

1. BaseAddress와 AllocationBase가 무슨 차이냐?

Allocation(전체 메모리): 아파트 단지 전체의 시작 주소. Winodows는 메모리를 할당할 때 보통 64KB(0x100000) 단위로 크게 예약.

BaseAddress(현재 구역): 아파트 단지 내의 특정 동/호수나 특정 구역의 시작 주소입니다.


왜 나뉘어 있는데?

하나의 커다란 할당 영역(AllocationBase) 안에서도 어떤 페이지는 읽기 전용이고 어떤 페이지는 읽기/쓰기 일 수 있습니다.

VirtualQuery는 속성이 바뀌는 지점마다 끊어서 정보를 주는데, 이때 “지금 보고 있는 이 속성이  시작되는 지점”이 BaseAddress입니다.


----------------------------------------------------------------------------------------------------------------------------------------------------------


2. AllocationProtect와 Protect는 권한인데 처음 할당될 때 권한이 달라져서 Protect에서 다른 값이 나올 수 있나?

AllocationProtect: 처음에 땅을 빌릴 때(예약할 때) 설정한 최초의 권한.                                                  ex) 여기는 읽고 쓰는 용도로 쓰겠다”

Protect: 현재 이 구역이 가지고 있는 실제 권한.



권한이 달라지는 대표적인 경우가 뭐냐?

동적 코드 생성: 처음에는 데이터 저장을 위해 PAGE_READWRITE로 빌렸다가, 나중에 코드를 집어넣고 VirtualProtect 함수를 써서 PAGE_EXECUTE_READ로 권한을 바꿀 수 있습니다.

보안 강화(DEP): 실행이 필요 없는 데이터 영역에 누군가 코드를 심어 실행하지 못하도록 시스템이 강제로 권한을 변경하기도 합니다.


----------------------------------------------------------------------------------------------------------------------------------------------------------


3. MEMORY_BASIC_INFORMATION - 함수를 호출하고 나면, 선언한 이 구조체 안에 해당 영역의 정보가 담깁니다.

MEMORY_BASIC_INFORMATION mbi; 로 선언.

BaseAddress / 영역의 시작 주소 / 현재 보고 있는 블록의 시작 위치.
RegionSize  / 영역의 크기     /  다음 영역으로 건너뛸 때 이만큼 더함.
state       / 메모리 상태     / MEM_COMMIT 상태인지 MEM_FREE 상태인지 확인
Protect     / 접근 권한       / PAGE_READONLY, PAGE_READWRITE 등을 확인.
tYPE        / 메모리 타입     / MEM_IMAGE(이미지 파일), MEM_PRIVATE(개인 데이터) 등을 구분.


----------------------------------------------------------------------------------------------------------------------------------------------------------


4. VitualQuery 생김새.

SIZE_T VirtualQuery(
    LPCVOID                   lpAddress,    //[입력] 궁금한 메모리 주소
    PMEMORY_BASIC_INFORMATION lpBuffer,     //[출력] 정보를 담아둘 구조체 변수의 주소
    SIZE_T                    dwLength,     //[입력] 구조체의 크기 (sizeof)
);

VitualQuery는 위와 같이 생김. 그러므로 VirtualQuery(주소, 정보를 담아둘 구조체 변수의 주소, 주조체 크기)처럼 작성.
ex)  VirtualQuery(addr_min, &mbi, sizeof(mbi))
