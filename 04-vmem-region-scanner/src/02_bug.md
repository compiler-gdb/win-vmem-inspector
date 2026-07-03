2026.05.16 22:23

bug: 경계가 바뀔 때마다 구간을 출력하려고 했지만, 출력 완료 마지막 시점의 3번째 줄에 구간 3개가 몰려서 출력됩니다.

다음은 


COMMIT
Address: 00007FFC8B546FF9 | 00 
Address: 00007FFC8B546FFA | 00 


RESERVE
Address: 00007FFC8B546FFB | 00 
Address: 00007FFC8B546FFC | 00 


FREE
Address: 00007FFC8B546FFD | 00 
Address: 00007FFC8B546FFE | 00 

03에서는 같은 형식으로 경계가 바뀔 때마다 경계를 출력 후에 데이터를 출력하기 시작하도록 하는것을 목표로 합니다.