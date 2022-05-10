#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <list>
using namespace std;

typedef struct
{
	int name;//이름
    int arrive;//도착시간
    int service;//실행시간
    int priority;//우선순위
    int tq;//tq
} process;


class process{
public:
    process(int name, int arrive, int service, int priority, int tq);
};

int num;
int quantum = 1;

int* burst_time; // bursttime 배열
int* complete_time; // completetime 배열
int* wait_time; // waiting time 배열
int* turnaround_time; //turnaround time 배열

int main(){
    scanf("개수를 입력하시오: %d",&num);
    int name, arrive, service, priority, tq;

    process **Process;
    for(int i = 0;i<num;i++){
        std::cin>>name>>arrive>>service>>priority>>tq;
        Process[i] = new process(name, arrive, service, priority, tq);

    }

}
