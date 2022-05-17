#include <iostream>
#include <cstdlib>
#include <list>
#include <queue>
#include <string.h>
#include <vector>

using namespace std;

int num=0;
int TQ=0;

vector<processing>* processing_time;
int* burst_time; // bursttime 배열
int* complete_time; // completetime 배열
int* wait_time; // waiting time 배열
int* turnaround_time; //turnaround time 배열
//////// RR은 도착시간 순서이긴한데 TQ에 맞춰서 계속 바뀔 수 있다.
/*typedef struct
{
	int name;//이름
    int arrive;//도착시간
    int service;//실행시간
    int priority;//우선순위
    int tq;//tq
} process;*/
class Process{
    friend class set;
public:
    int name;
    int arrive;
    int service;
    int priority;
    Process(int name_=0, int arrive_ = 0, int service_ = 0, int priority_ = 0){
        //strcpy(name, name_);//strcpy_s 사용하면 더 좋음
        name = name_;
        arrive = arrive_;
        service = service_;
        priority = priority_;
    }
    //void RR();
};

typedef struct processing {
	int start;
	int end;
	int burst;
}processing;

class Set {
public:
    Process *head[10000];
    int n=0;
    Set(){
        /*head[n] = new Process();
        n++;*/
    }
    void insert(int name_=0, int arrive_ = 0, int service_ = 0, int priority_ = 0){
        //Process process[num];
        head[n] = new Process(name_, arrive_, service_, priority_);
        n++;
    }
    void sort();
    void RR();
};


void Set::sort(){
    int i, j;
    Process *temp = new Process();

    for(i = 1; i<num; i++){
        for(j=1; j<i; j++){
            if(head[j]->arrive < head[j+1]->arrive){
                temp = head[j];
                head[j] = head[j+1];
                head[j+1] = temp;
            }
        }
    }
}

//queue에 넣어야지
//도착순으로 정렬 됬으니까
//이제 queue 넣어
//근데 이제 TQ를 생각하면서
//만약 TQ가 2라고 한다면
//queue에 process[0]번째 2번 insert 해주고, service -= 2 해주고,O
//process[1]번째 2번 insert 해주고, service -= 2 해주고, O
//만약 service에 값이 0이라면, 거기는 이제 queue에 insert안해주고, 다음꺼를 찾는다. O
//만약 service 값이 1 이라고 하면, queue에 한번만 insert해주어야한다. O

//이렇게 반복인가?

//RR은 서비스 타임, TQ, 도착시간만 있으면됨
//TQ시간 만큼 빼야지,,,,,

//프로세스별 대기시간 및 평균 대기시간, 평균 반환시간, 각 프로세스별 응답시간 및 평균 응답시간

void Set::RR(){
    sort();
    int i=0;
    int temp=0;
    int sum=0;
    processing pt;
    int total = 0, complate = 0, turnaroun = 0, wait = 0;
    pt.start= 0;

    queue<int> queue;
    for(i = 0; i<num; i++){
        sum += head[i]->service;
    }

    while(1){
        pt.start = i;
        if(head[i]->service == 0){
            temp++;
        }
        else if(head[i]->service >= TQ){
            for(i = 0;i <head[i]->name;i++)
                queue.push(head[i]->name);
            head[i]->service = head[i]->service - TQ;
            temp=0;

        }
        else{
            for(i = 0; i<head[i]->service; i++)
                queue.push(head[i]->name);
            head[i]->service = 0;
            temp=0;

            pt.end = pt.start + head[i]->service;
            pt.burst = head[i]->service;
            if(pt.burst != 0)
                processing_time[i].push_back(pt);
            pt.start = pt.end;

        }
        i++;
        if(i==num){
            i = 0;
        }
        if(temp==num){
            break;
        }
    }

    while(!queue.empty()){
        cout<<queue.front()<<" ";
        queue.pop();
    }
}




int main(){
    int name;
    int arrive, service, priority, tq;
    Set *set = new Set();
    cout << "개수를 입력하시오: ";
    cin >> num;
    cout << "TQ를 입력하시오";
    cin >> TQ;

    //Process *process[num];
    for(int i = 0;i<num;i++){
        cout << "프로세스별 이름과 도착시간 서비스시간 우선순위 TQ를 입력하세요";
        cin>>name>>arrive>>service>>priority;
        set->insert(name, arrive, service, priority);
    }
    set->RR();
    //RR(**process);

}
