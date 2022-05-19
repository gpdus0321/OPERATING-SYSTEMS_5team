#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <list>
#include <queue>
#include <algorithm>
using namespace std;

typedef struct
{
	int name;
    int arrive;
    int service;
    int priority;
    int tq;
} process;

typedef struct processing {
	int start;
	int end;
	int burst;
}processing;

// 실행시간 정렬
struct cmpburst {
	bool operator()(process a, process b) {
		return a.service > b.service;
	}
};


int num;
int quantum = 1;
int fin_time = 0;

int* burst_time; // bursttime 
int* complete_time; // completetime 
int* wait_time; // waiting time 
int* turnaround_time; //turnaround time
int* response_time; // response time
vector<processing>* processing_time; //각 프로세스 별로 간트차트용 값


void FCFS( process *Process, int num ){
    
    // 시간 배열 선언
    burst_time = new int[num];
    complete_time = new int[num];
    wait_time = new int[num];
    turnaround_time = new int[num];
    response_time = new int[num];
    processing_time = new vector<processing>[num];

    for(int i=0; i<num-1; i++){
        for( int j = 0; j<num-i; j++){
            if( Process[j].arrive >Process[j+1].arrive){
                process temp = Process[j+1];
                Process[j+1] = Process[j];
                Process[j] = temp;
            } 
        }
    }

   
    int time = 0;
    processing tmp;
    
    int avg_wait = 0;
    int avg_turnaround = 0;
    int avg_response = 0;

    for(int i = 0; i< num; i++){

        int burst_t = Process[i].service;
        int arrive_t = Process[i].arrive;

        // 간트 차트용 값 작성
        tmp.burst = burst_t;
        tmp.start = arrive_t;
        tmp.end = burst_t + arrive_t;
        processing_time[i].push_back(tmp); 
       
        while(time < arrive_t){
            time += 1;
        }

        

        // 대기 시간
        wait_time[i] = time - arrive_t;
        avg_wait += time - arrive_t;

        // 반환 시간
        turnaround_time[i] = time + burst_t;
        avg_turnaround += time + burst_t;
        
        // 실행 시간
        burst_time[i] = burst_t;
        
        // 완료 시간
        complete_time[i] = time + burst_t;

        // 반응 시간
        response_time[i] = time -arrive_t;
        avg_response += time -arrive_t;

        time += burst_t; 
        fin_time = time;
    }

    avg_wait /= num;
    avg_turnaround /= num;
    avg_response /= num;

    std::cout << "avg_wait:" << avg_wait << endl;
    std::cout << "avg_turnaroud:" << avg_turnaround << endl;
    std::cout << "avvg_response:" << avg_response << endl;

}


void SJF( process *Process, int num ){
    
    burst_time = new int[num];
    complete_time = new int[num];
    wait_time = new int[num];
    turnaround_time = new int[num];
    response_time = new int[num];
    processing_time = new vector<processing>[num];
    
    // ready queue 
    priority_queue<process, vector<process>,cmpburst> readyQ;

    // 도착 시간이 빠른 순서대로 정렬
    for(int i=0; i<num-1; i++){
        for( int j = 0; j<num-i; j++){
            if( Process[j].arrive > Process[j+1].arrive){
                process temp = Process[j+1];
                Process[j+1] = Process[j];
                Process[j] = temp;
            }

            else if( (Process[j].service > Process[j+1].service) && (Process[j].arrive == Process[j+1].arrive) ){
                process temp = Process[j+1];
                Process[j+1] = Process[j];
                Process[j] = temp;
            }
        }
    }


    int time = 0;
    int avg_wait = 0;
    int avg_turnaround = 0;
    int avg_response = 0;

    list<int> done_list;
    std::list<int>::iterator it;
    

    int process_num = 0;

    while(time < fin_time){
        
        // 첫번째 프로세스는 가장 빠른 것 
        int burst_t = Process[process_num].service;
        int arrive_t = Process[process_num].arrive;

        //간트 차트 용 값 추가
        processing tmp;
        tmp.burst = burst_t;
        tmp.start = arrive_t;
        tmp.end = burst_t + arrive_t;
        processing_time[process_num].push_back(tmp);
        
        // 실행할 것이 없는 경우 진행
        while(time < arrive_t){
            time += 1;
        }

        // 프로세스 값에 따라 시간 계산
        wait_time[process_num] = arrive_t- time;
        turnaround_time[process_num] = burst_t + time;
        response_time[process_num] = burst_t - time;
        
        //현재 시간을 올려준다
        time += burst_t;


        // 시행되지 않았고 arrive가 시간보다 적은 것을 priority ready queue에 삽입 
        for(int i = 0; i< num; i++){
            
            //시행되지 않았는지 확인
            it = find(done_list.begin(),done_list.end(),Process[i].name);
            if((Process[i].arrive <= time + burst_t) && (it != done_list.end())) {
                
                //readyqueue에 삽입하고 done에서 값을 삭제
                readyQ.push(Process[i]);
                done_list.push_back(Process[i].name);
            }
        }
        // priority ready_queue에 들어간 것 중에서 우선순위 높은 값 뽑기
        int next_process = readyQ.top().name;
        readyQ.pop();

        // 다음 실행을 위해서 프로세스숫자를 다음 우선순위가 높은 프로세세 값으로 바꿈
        for(int i=0; i< num; i++){
            if(Process[i].name==next_process){
                process_num = i;
            }
        }
        
    }

    avg_wait /= num;
    avg_turnaround /= num;
    avg_response /= num;

    std::cout << "avg_wait:" << avg_wait << endl;
    std::cout << "avg_turnaroud:" << avg_turnaround << endl;
    std::cout << "avvg_response:" << avg_response << endl;

}


int main(){
    std::cout<<"Hello world\n";
    std::cout<<"개수를 입력해주세요: ";
    std::cin>>num;
    int name, arrive, service, priority, tq;
    process *Process = new process[num];
    for(int i = 0;i<num;i++){
        std::cout<<".";
        std::cin>>name>>arrive>>service>>priority>>tq;
        Process[i].name = name;
        Process[i].arrive = arrive;
        Process[i].priority = priority;
        Process[i].service = service;
        Process[i].tq = tq;
    }
    FCFS(Process, num);
}
