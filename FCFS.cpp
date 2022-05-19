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

// ����ð� ����
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
vector<processing>* processing_time; //�� ���μ��� ���� ��Ʈ��Ʈ�� ��


void FCFS( process *Process, int num ){
    
    // �ð� �迭 ����
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

        // ��Ʈ ��Ʈ�� �� �ۼ�
        tmp.burst = burst_t;
        tmp.start = arrive_t;
        tmp.end = burst_t + arrive_t;
        processing_time[i].push_back(tmp); 
       
        while(time < arrive_t){
            time += 1;
        }

        

        // ��� �ð�
        wait_time[i] = time - arrive_t;
        avg_wait += time - arrive_t;

        // ��ȯ �ð�
        turnaround_time[i] = time + burst_t;
        avg_turnaround += time + burst_t;
        
        // ���� �ð�
        burst_time[i] = burst_t;
        
        // �Ϸ� �ð�
        complete_time[i] = time + burst_t;

        // ���� �ð�
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

    // ���� �ð��� ���� ������� ����
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
        
        // ù��° ���μ����� ���� ���� �� 
        int burst_t = Process[process_num].service;
        int arrive_t = Process[process_num].arrive;

        //��Ʈ ��Ʈ �� �� �߰�
        processing tmp;
        tmp.burst = burst_t;
        tmp.start = arrive_t;
        tmp.end = burst_t + arrive_t;
        processing_time[process_num].push_back(tmp);
        
        // ������ ���� ���� ��� ����
        while(time < arrive_t){
            time += 1;
        }

        // ���μ��� ���� ���� �ð� ���
        wait_time[process_num] = arrive_t- time;
        turnaround_time[process_num] = burst_t + time;
        response_time[process_num] = burst_t - time;
        
        //���� �ð��� �÷��ش�
        time += burst_t;


        // ������� �ʾҰ� arrive�� �ð����� ���� ���� priority ready queue�� ���� 
        for(int i = 0; i< num; i++){
            
            //������� �ʾҴ��� Ȯ��
            it = find(done_list.begin(),done_list.end(),Process[i].name);
            if((Process[i].arrive <= time + burst_t) && (it != done_list.end())) {
                
                //readyqueue�� �����ϰ� done���� ���� ����
                readyQ.push(Process[i]);
                done_list.push_back(Process[i].name);
            }
        }
        // priority ready_queue�� �� �� �߿��� �켱���� ���� �� �̱�
        int next_process = readyQ.top().name;
        readyQ.pop();

        // ���� ������ ���ؼ� ���μ������ڸ� ���� �켱������ ���� ���μ��� ������ �ٲ�
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
    std::cout<<"������ �Է����ּ���: ";
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
