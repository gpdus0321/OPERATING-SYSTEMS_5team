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


int num;
int TQ = 0;
int fin_time = 0;

int* burst_time; // bursttime 
int* complete_time; // completetime 
int* wait_time; // waiting time 
int* turnaround_time; //turnaround time
int* response_time; // response time

vector<processing>* processing_time; //�� ���μ��� ���� ��Ʈ��Ʈ�� ��

void print_(double avr_wait, double avr_turnaround, double avr_response) {
    int i, j;

    cout << endl << "��Ʈ ��Ʈ" << endl;
    //��Ʈ��Ʈ�� start �������� �����ؼ� �� ������ �ɵ�.

    for (int i = 0; i < num; i++) {
        cout << "P" << i + 1 << " : ";
        for (int j = 0; j < processing_time[i].size(); j++)
            cout << "(" << processing_time[i][j].start << " ~ " << processing_time[i][j].end << " " << processing_time[i][j].burst << "����) ";
        cout << endl;
    }

    //���μ����� ���ð�
    cout << "���μ����� ���ð� : ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << wait_time[i] << "  ";
    cout << endl;
    //���μ����� ��ȯ�ð�
    cout << "���μ����� ��ȯ�ð� : ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << turnaround_time[i] << "  ";
    cout << endl;
    //���μ����� ����ð�
    cout << "���μ����� ����ð� : ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << response_time[i] << "  ";

    cout << endl;
    //��մ��ð�
    cout << "��� ���ð� : " << avr_wait << endl;
    //��չ�ȯ�ð�
    cout << "��� ��ȯ�ð� : " << avr_turnaround << endl;
    //�������ð�
    cout << "��� ����ð� : " << avr_response << endl;
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
    
    print_(avg_wait, avg_turnaround, avg_response);

}

int main(){
   
    int name, arrive, burst, priority;

    cout << "������ �Է��Ͻÿ�: ";
    cin >> num;
    cout << "TQ�� �Է��Ͻÿ�: ";
    cin >> TQ;

    process *Process = new process[num];
    processing_time = new vector<processing>[num]();
    burst_time = new int[num]();
    complete_time = new int[num]();
    wait_time = new int[num]();
    turnaround_time = new int[num]();
    response_time = new int[num]();


    for(int i = 0;i<num;i++){
        std::cout<<"���μ����� ariive burst priority �Է��ϼ���:";
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
