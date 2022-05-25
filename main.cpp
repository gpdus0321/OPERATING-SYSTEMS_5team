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

// ����ð� ����
struct cmpburst {
	bool operator()(process a, process b) {
		return a.burst > b.burst;
	}
};



int num = 0;
int TQ = 0;
int fin_time = 0;

int* burst_time; // bursttime 
int* complete_time; // completetime 
int* wait_time; // waiting time 
int* turnaround_time; //turnaround time
int* response_time; // response time
vector<processing>* processing_time; //�� ���μ��� ���� ��Ʈ��Ʈ�� ��

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
        turnaround_time[i] = time + burst_t - arrive_t;
        avg_turnaround += time + burst_t -arrive_t;
        
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

void SJF( process *Process){
    
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

        
        // ���� ���μ���
        int burst_t = Process[process_num].burst;
        int arrive_t = Process[process_num].arrive;
        
        //������ ���� ���� ��Ȳ
        if(arrive_t > time){
            time = arrive_t;
        }

        //��Ʈ ��Ʈ �� �� �߰�
        processing tmp;
        
        tmp.start = time;
        tmp.end = burst_t + time;
        tmp.burst = tmp.start - tmp.end;
        processing_time[process_num].push_back(tmp);
        
    
        // ���μ��� ���� ���� �ð� ���
        wait_time[process_num] =  time - arrive_t;
        avg_wait +=  (time - arrive_t) ;

        turnaround_time[process_num] = burst_t + time -arrive_t;
        avg_turnaround += burst_t + time- arrive_t;

        response_time[process_num] = time - arrive_t;
        avg_response += time - arrive_t;
        
        //���� �ð��� �÷��ش�
        time += burst_t;

        // ������� �ʾҰ� arrive�� �ð����� ���� ���� priority ready queue�� ���� 
        for(int i = 0; i< num; i++){
            
            //������� �ʾҴ��� Ȯ��
            if((Process[i].arrive <= time) && (done_find(done,done_num,Process[i].name) == false)) {
                
                //readyqueue�� �����ϰ� done���� ���� �߰�
                readyQ.push(Process[i]);
                                
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

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;
    
    print_(avg_wait, avg_turnaround, avg_response);

}
/*
void SRTF( process *Process ){


    //�ʱ� ���� �δ� ���� �Ȱ��� 
    //done�� ���ְ� ���μ����� ����Ʈ ���� ��Ƽ� 0�� �� ��� �������� �ʴ� ���
    // bust���� �δ� �迭�� ���� �ʿ䰡 ����
    
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

    // ���μ������� ����Ʈ ���� ����
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
            
            //�� �� �������� ���μ����� ����Ʈ �� ����
            Process[process_num].burst -=1;
            // �ð� ����
            time +=1;

            // arrive�� �ð����� ���� ���� priority ready queue�� ���� 
            for(int i = 0; i< num; i++){
                if(Process[i].arrive <= time  &&  Process[i].burst > 0 ){
                    //readyqueue�� �����ϰ� 
                    readyQ.push(Process[i]);
                }
            }
            
            // ready que�� �� ���� ���� ������ ����ϰ� ��ä
            if(readyQ.top().burst < burst_t){
                
                //��Ʈ ��Ʈ �� �� �߰�
                processing tmp;
                tmp.start = start_t;
                tmp.end = time;
                tmp.burst = tmp.start - tmp.end;
                processing_time[process_num].push_back(tmp);

                // ���μ��� ���� ���� �ð� ���
                wait_time[process_num] +=  time - start_t;
                avg_wait +=  (time - start_t) ;

                turnaround_time[process_num] += tmp.start - tmp.end + time -start_t;
                avg_turnaround += tmp.start - tmp.end + time- start_t;

                //response�� ó�� �� ���� ����
                if(response_time[process_num]<= time - arrive_t){
                    response_time[process_num] = time - arrive_t;
                    avg_response += time - arrive_t;
                }

                // ���� ���μ����� Ǫ��
                readyQ.push(Process[process_num]);

                //readyque���� ���ο� ���μ����� ������
                process next_process = readyQ.top();
                readyQ.pop();
                process_num = next_process.name;
                break;
            }
            //���μ����� ���� �ð��� 0�̵Ǿ� ����Ǵ� ���
            else if(Process[process_num].burst ==0){
                
                //��Ʈ ��Ʈ �� �� �߰�
                processing tmp;
                tmp.start = start_t;
                tmp.end = time;
                tmp.burst = tmp.start - tmp.end;
                processing_time[process_num].push_back(tmp);

                // ���μ��� ���� ���� �ð� ���
                wait_time[process_num] +=  time - start_t;
                avg_wait +=  (time - start_t) ;

                turnaround_time[process_num] += tmp.start - tmp.end + time -start_t;
                avg_turnaround += tmp.start - tmp.end + time- start_t;

                //response�� ó�� �� ���� ����
                if(response_time[process_num]<= time - arrive_t){
                    response_time[process_num] = time - arrive_t;
                    avg_response += time - arrive_t;
                }

                //readyque���� ���ο� ���μ����� ������
                process next_process = readyQ.top();
                readyQ.pop();
                process_num = next_process.name;
                break;


            }
            

        }
    
       
    
        // arrive�� �ð����� ���� ���� priority ready queue�� ���� 
        for(int i = 0; i< num; i++){
            
            if(Process[i].arrive <= time){

                //readyqueue�� �����ϰ� 
                readyQ.push(Process[i]);
                                
            }
        }
        // priority ready_queue�� �� �� �߿��� �켱���� ���� �� �̱�
        process next_process = readyQ.top();
        readyQ.pop();

        //������ ���� ���� ��Ȳ
        if(arrive_t > time){
            time = arrive_t;
        }

        //��Ʈ ��Ʈ �� �� �߰�
        processing tmp;
        
        tmp.start = time;
        tmp.end = burst_t + time;
        tmp.burst = tmp.start - tmp.end;
        processing_time[process_num].push_back(tmp);
        
    
        // ���μ��� ���� ���� �ð� ���
        wait_time[process_num] =  time - arrive_t;
        avg_wait +=  (time - arrive_t) ;

        turnaround_time[process_num] = burst_t + time -arrive_t;
        avg_turnaround += burst_t + time- arrive_t;

        response_time[process_num] = time - arrive_t;
        avg_response += time - arrive_t;
        
        //���� �ð��� �÷��ش�
        time += 1;

        // �ð��� �ϳ��� �ø��鼭 ready que ���� Ȯ���ϸ鼭 ��ü�� ���μ����� ���ᰡ �߻��ϴ��� Ȯ��




      
    }

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;
}

*/
int main() {
    int name;
    int arrive, burst, priority;

    cout << "개수를 입력하시오: ";
    cin >> num;
    cout << "TQ를 입력하시오: ";
    cin >> TQ;

    process* Process = new process[num];

    processing_time = new vector<processing>[num]();
    burst_time = new int[num]();
    complete_time = new int[num]();
    wait_time = new int[num]();
    turnaround_time = new int[num]();
    response_time = new int[num]();

    for (int i = 0; i < num; i++) {
        cout << "프로세스별 ariive burst priority 입력하세요: ";
        cin >> arrive >> burst >> priority;
        Process[i].name = i;
        Process[i].arrive = arrive;
        Process[i].priority = priority;
        Process[i].burst = burst;
        burst_time[i] = burst;
    }
    FCFS(Process);
    SJF(Process);
    //SRTF(Process);
    RR(Process);
    //Priority(Process);

    delete[]wait_time;
    delete[]processing_time;
    delete[]complete_time;
    delete[]turnaround_time;
    delete[]burst_time;
    delete[]response_time;

    return 0;
}


void RR(process* Process) {

    sort_arrive(Process);

    int i, sum = 0, complete = 0, turnaround = 0, wait = 0;
    process ing = {};
    processing pt = {};

    double avr_wait = 0.0, avr_turnaround = 0.0, avr_response = 0.0;
    queue<process> Plist = {};
    queue<process> readyQ = {};

    for (i = 0; i < num; i++)
        processing_time[i].clear();


    for (i = 0; i < num; i++) {
        Plist.push(Process[i]);
        sum += Process[i].burst;
    }
    i = 0;
    while (i < sum) {

        while (1) {
            if (Plist.size() != 0 && Plist.front().arrive == i) {
                readyQ.push(Plist.front());
                Plist.pop();
            }
            else break;
        }

        ing = readyQ.front();
        readyQ.pop();

        int name_ = ing.name;
        int burst_ = ing.burst;
        int arrive_ = ing.arrive;
        pt.start = i;

        //큰경우
        if (burst_ > TQ) {
            while (1) {
                if (Plist.size() != 0 && Plist.front().arrive < i + TQ) {
                    readyQ.push(Plist.front());
                    Plist.pop();
                }
                else
                    break;
            }

            pt.end = pt.start + TQ;
            pt.burst = TQ;
            processing_time[name_].push_back(pt);
            pt.start = pt.end;

            ing.burst -= TQ;
            readyQ.push(ing);
            i = i + TQ;

        }
        //작은경우
        else {
            while (1) {
                if (Plist.size() != 0 && Plist.front().arrive < i + burst_) {
                    readyQ.push(Plist.front());
                    Plist.pop();
                }
                else
                    break;
            }

            pt.end = pt.start + burst_;
            pt.burst = burst_;
            if (pt.burst != 0)
                processing_time[name_].push_back(pt);
            pt.start = pt.end;

            complete = i + burst_;
            turnaround = complete - arrive_;
            wait = complete - arrive_ - burst_time[name_];
            complete_time[name_] = complete;
            turnaround_time[name_] = turnaround;
            wait_time[name_] = wait;

            avr_wait += (double)wait;
            avr_turnaround += (double)turnaround;

            i = i + burst_;
        }
    }
    for (i = 0; i < num; i++) {
        response_time[i] = processing_time[i][0].start - Process[i].arrive;
        avr_response += response_time[i];
    }

    avr_wait /= (double)num;
    avr_turnaround /= (double)num;
    avr_response /= (double)num;

    print_(avr_wait, avr_turnaround, avr_response);
}
// 비선점 Priority이면 우선순위 정렬이 필요함 => sort_priority
// 근데 선점이랑 다르게 이거는 한번 실행하면 종료할때까지 실행한다.
// 그렇다고 우선순위 순으로 계속 하는게 아니라, 0초에 시작하는 것 중에 우선순위 높은거 실행하고
// 그 프로세스가 끝나고 난 시간에 우선순위 높은거 실행하고, 그런식으로 진행해야함.
// 그럼 일단 진행 시간 확인해야지
void Priority(process* Process) {
    sort_priority(Process);

    int i, complete = 0, turnaround = 0, wait = 0;
    process ing = {};
    processing pt = {};

    double avr_wait = 0.0, avr_turnaround = 0.0, avr_response = 0.0;
    queue<process> Plist = {};

    for (i = 0; i < num; i++)
        processing_time[i].clear();

    for (i = 0; i < num; i++)
        Plist.push(Process[i]);

    while (1) {

    }

}

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

void sort_priority(process* Process) {
    int i, j;

    for (i = 0; i < num - 1; i++) {
        for (j = 0; j < num - i; j++) {
            if (Process[j].priority > Process[j + 1].priority) {
                process temp = Process[j + 1];
                Process[j + 1] = Process[j];
                Process[j] = temp;
            }
        }
    }
}