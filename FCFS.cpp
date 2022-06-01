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
        if(a.burst == b.burst){
            return a.arrive > b.arrive;
        }
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
        tmp.start = time;
        tmp.end = burst_t + time;
        int name = Process[i].name;
        processing_time[name].push_back(tmp); 
       
        while(time < arrive_t){
            time += 1;
        }

        
        // 대기 시간
        wait_time[name] = time - arrive_t;
        avg_wait += time - arrive_t;

        // 반환 시간
        turnaround_time[name] = time + burst_t - arrive_t;
        avg_turnaround += time + burst_t -arrive_t;
        
        // 실행 시간
        burst_time[name] = burst_t;
        
        // 완료 시간
        complete_time[name] = time + burst_t;

        // 반응 시간
        response_time[name] = time -arrive_t;
        avg_response += time -arrive_t;

        time += burst_t; 
        fin_time = time;
    }

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;
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
    printf("start");
    priority_queue<process, vector<process>,cmpburst> readyQ;

    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    // 도착 순서대로 정렬 후, 우선 순위 적용
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
    process present = Process[process_num];
    int name = present.name;
    

    while(time < fin_time){

        // 현재 프로세스를 정함

        name = present.name;
        done[done_num] = name;
        done_num += 1;

        
        // 현재 프로세스
        int burst_t = present.burst;
        int arrive_t = present.arrive;
        
        //실행할 것이 없는 상황
        if(arrive_t > time){
            time = arrive_t;
        }

        //간트 차트 용 값 추가
        processing tmp;
        
        tmp.start = time;
        tmp.end = burst_t + time;
        tmp.burst = burst_t;
        processing_time[name].push_back(tmp);
        

    
        // 프로세스 값에 따라 시간 계산
        wait_time[name] =  time - arrive_t;
        avg_wait +=  (time - arrive_t) ;

        turnaround_time[name] = burst_t + time -arrive_t;
        avg_turnaround += burst_t + time- arrive_t;

        response_time[name] = time - arrive_t;
        avg_response += time - arrive_t;
        
        //현재 시간을 올려준다
        time += burst_t;

        printf("현재 프로세스 %d\n", name);

        // 시행을 완료 했고, 하지 않은 것들 중에서 새로운 것 고르기
        // 시행되지 않았고 arrive가 시간보다 적은 것을 priority ready queue에 삽입 
        for(int i = 0; i< num; i++){
            priority_queue<process, vector<process>,cmpburst> readyQ2;
            readyQ2 = readyQ;
            bool isin = false;
            for(int j = 0; j <readyQ.size(); j++){
                process tmp = readyQ2.top();
                readyQ2.pop();
                if(tmp.name == Process[i].name){
                    isin = true;
                }
            }
            
            //시행되지 않았는지 확인
            if((Process[i].arrive <= time) && (done_find(done,done_num,Process[i].name) == false) && (isin == false)) {
                printf("레디큐 프로세스: %d\n",Process[i].name);
                //readyqueue에 삽입하고 done에서 값을 추가
                readyQ.push(Process[i]);
                                
            }
        }
        // priority ready_queue에 들어간 것 중에서 우선순위 높은 값 뽑기
        int next_process = readyQ.top().name;
        

        readyQ.pop();

        // 다음 실행을 위해서 프로세스숫자를 다음 우선순위가 높은 프로세세 값으로 바꿈
        for(int i=0; i < num; i++){
            if(Process[i].name == next_process){
                process_num = i;
            }
        }

        present = Process[process_num];
        
    }

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;
    
    print_(avg_wait, avg_turnaround, avg_response);

}


void SRTF(process *Process){

    priority_queue<process, vector<process>,cmpburst> readyQ;

    int burst[num];
    for (int i =0; i< num; i++){
        burst[i] = Process[i].burst;
    }


    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    // 도착 순서에 따라서 배열
    sort_arrive(Process);

    // 도착 순서가 같다면, 실행 시간이 짧은 것을 우선
    for (int i = 0; i < num - 1; i++) {
        for (int j = 0; j < num - i; j++) {
            if (Process[j].arrive==Process[j+1].arrive && Process[j].burst>Process[j+1].burst) {
                process temp = Process[j + 1];
                Process[j + 1] = Process[j];
                Process[j] = temp;
            }
        }
    }

    int total = 0, complete = 0, turnaround = 0, wait = 0;
    int time = 0;
    double avg_wait = 0;
    double avg_turnaround = 0;
    double avg_response = 0;
    int process_num = 0;
    int done[num];
    int done_num = 0;

    queue<process> Slist; 

	for (int j = 0; j < num; j++) {
		Slist.push(Process[j]);
		total += Process[j].burst;
	}
    process present, tmp;
    // 초기 프로세스 설정
	present = Slist.front();
	Slist.pop();
	processing pt;
	pt.start = 0;


    while(time<=fin_time){

        // cout << "현재 프로세스: " << present.name << endl;
        // printf("현재 프로세스: %d\n",present.name);
        // printf("현재 프로세스 버스트: %d\n",present.burst);
        // printf("현재 시간%d\n",time);

        while (1){
            if (Slist.size() != 0 && Slist.front().arrive == time){
                tmp = Slist.front();
			    Slist.pop();
                if (tmp.burst < present.burst) {

					// 현재 프로세스 교체되면 processing time 계산
					pt.end = time;
					pt.burst = time - pt.start;
					if (pt.burst != 0)
						processing_time[present.name].push_back(pt);
					pt.start = time; // 새로운 시작값 저장

					// 새로운 프로세스 꺼내옴
					readyQ.push(present);
					present = tmp;
				}
                else readyQ.push(tmp);
            }
            else break;

        }
        if (present.burst == 0) {
            int name = present.name;
			int burst = present.burst;
			int arrive = present.arrive;
            complete = time;

            if(done_find(done,done_num,name) == false){
                done[done_num] = name;
                done_num +=1;
                avg_response += complete - burst_time[name] - arrive;
                response_time[name] = complete - burst_time[name] - arrive;
            }

            
			wait = complete - burst_time[name] - arrive;
			turnaround = complete - arrive;
			turnaround_time[name] = turnaround;
			wait_time[name] = wait;

            avg_wait += (double)wait;
			avg_turnaround += (double)turnaround;

            pt.end = time;
			pt.burst = time - pt.start;
            if (pt.burst != 0)
				processing_time[name].push_back(pt);
			pt.start = time; // 새로운 시작값 저장
            if (readyQ.size() != 0) {
				present = readyQ.top();
				readyQ.pop();
		    }
            else if (time != fin_time && readyQ.size() == 0) {
				cout << "SJF 스케줄링 중간에 공백이 생겼습니다. - 잘못된 입력입니다.";
				system("pause");
				exit(1);
		    }
		    else break;
        }
        present.burst -= 1; // 현재 처리중인 프로세스의 burst 1 감소
	    time = time + 1; // 시간은 1 지남
    }
    avg_turnaround /= (double)num;
	avg_wait /= (double)num;
    avg_response /= (double)num;

    for( int i = 0; i <num; i++){
        Process[i].burst = burst[i];
    }

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
