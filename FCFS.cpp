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
    int burst;
    int priority;
} process;

typedef struct processing {
	int start;
	int end;
	int burst;
}processing;

// 실행시간 정렬
struct cmpburst {
	bool operator()(process a, process b) {
		return a.burst > b.burst;
	}
};


int num;
int TQ = 0;
int fin_time = 0;

int* burst_time; // bursttime 
int* complete_time; // completetime 
int* wait_time; // waiting time 
int* turnaround_time; //turnaround time
int* response_time; // response time

vector<processing>* processing_time; //각 프로세스 별로 간트차트용 값

void print_(double avr_wait, double avr_turnaround, double avr_response) {
    int i, j;

    cout << endl << "간트 차트" << endl;
    //간트차트는 start 오름차순 정렬해서 죽 뽑으면 될듯.

    for (int i = 0; i < num; i++) {
        cout << "P" << i + 1 << " : ";
        for (int j = 0; j < processing_time[i].size(); j++)
            cout << "(" << processing_time[i][j].start << " ~ " << processing_time[i][j].end << " " << processing_time[i][j].burst << "실행) ";
        cout << endl;
    }

    //프로세스별 대기시간
    cout << "프로세스별 대기시간 : ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << wait_time[i] << "  ";
    cout << endl;
    //프로세스별 반환시간
    cout << "프로세스별 반환시간 : ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << turnaround_time[i] << "  ";
    cout << endl;
    //프로세스별 응답시간
    cout << "프로세스별 응답시간 : ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << response_time[i] << "  ";

    cout << endl;
    //평균대기시간
    cout << "평균 대기시간 : " << avr_wait << endl;
    //평균반환시간
    cout << "평균 반환시간 : " << avr_turnaround << endl;
    //평균응답시간
    cout << "평균 응답시간 : " << avr_response << endl;
}

void sort_arrive(process* Process) {
    int i, j;

    for (i = 0; i < num - 1; i++) {
        for (j = 0; j < num - i; j++) {
            if (Process[j].arrive > Process[j + 1].arrive) {
                process temp = Process[j + 1];
                Process[j + 1] = Process[j];
                Process[j] = temp;
            }
        }
    }
}

void FCFS( process *Process ){
    
    
   sort_arrive(Process);

   
    int time = 0;
    processing tmp;
    
    double avg_wait = 0.0;
    double avg_turnaround = 0.0;
    double avg_response = 0.0;

    for (int i = 0; i < num; i++)
        processing_time[i].clear();


    for(int i = 0; i< num; i++){

        int burst_t = Process[i].burst;
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
        turnaround_time[i] = time + burst_t - arrive_t;
        avg_turnaround += time + burst_t -arrive_t;
        
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

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;

    printf("%d\n",fin_time);
    print_(avg_wait, avg_turnaround, avg_response);

}

bool done_find(int done[], int done_len, int num){
    for(int i =0 ; i<done_len; i++){
        if(done[i] == num){
            return true;
        }
    }
    return false;
}



void SJF( process *Process ){
    
    // ready queue 
    priority_queue<process, vector<process>,cmpburst> readyQ;

    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    sort_arrive(Process);
    for (int i = 0; i < num - 1; i++) {
        for (int j = 0; j < num - i; j++) {
            if (Process[j].arrive==Process[j+1].arrive && Process[j].burst>Process[j+1].burst) {
                process temp = Process[j + 1];
                Process[j + 1] = Process[j];
                Process[j] = temp;
            }
        }
    }

    int time = 0;
    double avg_wait = 0;
    double avg_turnaround = 0;
    double avg_response = 0;

    int done[num];
    int done_num = 0;

    int process_num = 0;
    

    while(time<fin_time){

        done[done_num] = Process[process_num].name;
        done_num += 1;

        
        // 현재 프로세스
        int burst_t = Process[process_num].burst;
        int arrive_t = Process[process_num].arrive;
        
        //실행할 것이 없는 상황
        if(arrive_t > time){
            time = arrive_t;
        }

        //간트 차트 용 값 추가
        processing tmp;
        
        tmp.start = time;
        tmp.end = burst_t + time;
        tmp.burst = tmp.start - tmp.end;
        processing_time[process_num].push_back(tmp);
        
    
        // 프로세스 값에 따라 시간 계산
        wait_time[process_num] =  time - arrive_t;
        avg_wait +=  (time - arrive_t) ;

        turnaround_time[process_num] = burst_t + time -arrive_t;
        avg_turnaround += burst_t + time- arrive_t;

        response_time[process_num] = time - arrive_t;
        avg_response += time - arrive_t;
        
        //현재 시간을 올려준다
        time += burst_t;

        // 시행되지 않았고 arrive가 시간보다 적은 것을 priority ready queue에 삽입 
        for(int i = 0; i< num; i++){
            
            //시행되지 않았는지 확인
            if((Process[i].arrive <= time) && (done_find(done,done_num,Process[i].name) == false)) {
                
                //readyqueue에 삽입하고 done에서 값을 추가
                readyQ.push(Process[i]);
                                
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

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;
    
    print_(avg_wait, avg_turnaround, avg_response);

}

void SRTF( process *Process ){


    //초기 값을 두는 것은 똑같음 
    //done을 없애고 프로세스의 버스트 값을 깍아서 0이 된 경우 실행하지 않는 방식
    // bust값을 두는 배열을 만들 필요가 있음
    
    // ready queue 
    priority_queue<process, vector<process>,cmpburst> readyQ;

    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    sort_arrive(Process);
    for (int i = 0; i < num - 1; i++) {
        for (int j = 0; j < num - i; j++) {
            if (Process[j].arrive==Process[j+1].arrive && Process[j].burst>Process[j+1].burst) {
                process temp = Process[j + 1];
                Process[j + 1] = Process[j];
                Process[j] = temp;
            }
        }
    }

    int time = 0;
    double avg_wait = 0;
    double avg_turnaround = 0;
    double avg_response = 0;

    // 프로세스들의 버스트 값을 저장
    int burst[num];
    for (int i =0; i< num; i++){
        burst[i] = Process[i].burst;
    }


    int process_num = 0;
    int burst_t = Process[process_num].burst;
    int arrive_t = Process[process_num].arrive;
    

    while(time<fin_time){
        
        int start_t = time;


        while(1){
            
            //한 번 루프마다 프로세스의 버스트 값 감소
            Process[process_num].burst -=1;
            // 시간 증가
            time +=1;

            // arrive가 시간보다 적은 것을 priority ready queue에 삽입 
            for(int i = 0; i< num; i++){
                if(Process[i].arrive <= time  &&  Process[i].burst > 0 ){
                    //readyqueue에 삽입하고 
                    readyQ.push(Process[i]);
                }
            }
            
            // ready que에 더 작은 값이 들어오면 기록하고 교채
            if(readyQ.top().burst < burst_t){
                
                //간트 차트 용 값 추가
                processing tmp;
                tmp.start = start_t;
                tmp.end = time;
                tmp.burst = tmp.start - tmp.end;
                processing_time[process_num].push_back(tmp);

                // 프로세스 값에 따라 시간 계산
                wait_time[process_num] +=  time - start_t;
                avg_wait +=  (time - start_t) ;

                turnaround_time[process_num] += tmp.start - tmp.end + time -start_t;
                avg_turnaround += tmp.start - tmp.end + time- start_t;

                //response는 처음 한 번만 적용
                if(response_time[process_num]<= time - arrive_t){
                    response_time[process_num] = time - arrive_t;
                    avg_response += time - arrive_t;
                }

                // 현재 프로세스를 푸시
                readyQ.push(Process[process_num]);

                //readyque에서 새로운 프로세스를 꺼내옴
                process next_process = readyQ.top();
                readyQ.pop();
                process_num = next_process.name;
                break;
            }
            //프로세스의 시행 시간이 0이되어 종료되는 경우
            else if(Process[process_num].burst ==0){
                
                //간트 차트 용 값 추가
                processing tmp;
                tmp.start = start_t;
                tmp.end = time;
                tmp.burst = tmp.start - tmp.end;
                processing_time[process_num].push_back(tmp);

                // 프로세스 값에 따라 시간 계산
                wait_time[process_num] +=  time - start_t;
                avg_wait +=  (time - start_t) ;

                turnaround_time[process_num] += tmp.start - tmp.end + time -start_t;
                avg_turnaround += tmp.start - tmp.end + time- start_t;

                //response는 처음 한 번만 적용
                if(response_time[process_num]<= time - arrive_t){
                    response_time[process_num] = time - arrive_t;
                    avg_response += time - arrive_t;
                }

                //readyque에서 새로운 프로세스를 꺼내옴
                process next_process = readyQ.top();
                readyQ.pop();
                process_num = next_process.name;
                break;


            }
            

        }
    
       
    
        // arrive가 시간보다 적은 것을 priority ready queue에 삽입 
        for(int i = 0; i< num; i++){
            
            if(Process[i].arrive <= time){

                //readyqueue에 삽입하고 
                readyQ.push(Process[i]);
                                
            }
        }
        // priority ready_queue에 들어간 것 중에서 우선순위 높은 값 뽑기
        process next_process = readyQ.top();
        readyQ.pop();

        //실행할 것이 없는 상황
        if(arrive_t > time){
            time = arrive_t;
        }

        //간트 차트 용 값 추가
        processing tmp;
        
        tmp.start = time;
        tmp.end = burst_t + time;
        tmp.burst = tmp.start - tmp.end;
        processing_time[process_num].push_back(tmp);
        
    
        // 프로세스 값에 따라 시간 계산
        wait_time[process_num] =  time - arrive_t;
        avg_wait +=  (time - arrive_t) ;

        turnaround_time[process_num] = burst_t + time -arrive_t;
        avg_turnaround += burst_t + time- arrive_t;

        response_time[process_num] = time - arrive_t;
        avg_response += time - arrive_t;
        
        //현재 시간을 올려준다
        time += 1;

        // 시간을 하나씩 올리면서 ready que 값을 확인하면서 교체나 프로세스의 종료가 발생하는지 확인




      
    }

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;
    
    print_(avg_wait, avg_turnaround, avg_response);

}

int main(){
   
    int name, arrive, burst, priority;

    cout << "개수를 입력하시오: ";
    cin >> num;
    cout << "TQ를 입력하시오: ";
    cin >> TQ;

    process *Process = new process[num];
    processing_time = new vector<processing>[num]();
    burst_time = new int[num]();
    complete_time = new int[num]();
    wait_time = new int[num]();
    turnaround_time = new int[num]();
    response_time = new int[num]();


    for(int i = 0;i<num;i++){
        std::cout<<"프로세스별 ariive burst priority 입력하세요:";
        std::cin>>arrive>>burst>>priority;
        Process[i].name = i;
        Process[i].arrive = arrive;
        Process[i].priority = priority;
        Process[i].burst = burst;
        burst_time[i] = burst;
    }
    FCFS(Process);
    SJF(Process);
    SRTF(Process);

    delete[]wait_time;
    delete[]processing_time;
    delete[]complete_time;
    delete[]turnaround_time;
    delete[]burst_time;
    delete[]response_time;


    
    return 0;
}
