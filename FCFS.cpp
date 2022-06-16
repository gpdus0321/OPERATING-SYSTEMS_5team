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

vector<processing>* processing_time; //�� ���μ��� ���� ��Ʈ��Ʈ�� ��

void print_(double avr_wait, double avr_turnaround, double avr_response) {
    int i, j;

    cout << endl << "간트 차트" << endl;
    //��Ʈ��Ʈ�� start �������� �����ؼ� �� ������ �ɵ�.

    for (int i = 0; i < num; i++) {
        cout << "P" << i + 1 << " : ";
        for (int j = 0; j < processing_time[i].size(); j++)
            cout << "(" << processing_time[i][j].start << " ~ " << processing_time[i][j].end << " " << processing_time[i][j].burst << "����) ";
        cout << endl;
    }


    cout << "프로세스별 대기시간 ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << wait_time[i] << "  ";
    cout << endl;
    //���μ����� ��ȯ�ð�
    cout <<"프로세스별 반환시간 : ";
    for (i = 0; i < num; i++)
        cout << "P" << i << " - " << turnaround_time[i] << "  ";
    cout << endl;
    //���μ����� ����ð�
    cout << "프로세스별 응답시간 : ";
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
        for (j = 0; j < num - i- 1; j++) {
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
        tmp.start = time;
        tmp.end = burst_t + time;
        int name = Process[i].name;
        processing_time[name].push_back(tmp); 
       
        while(time < arrive_t){
            time += 1;
        }

        
        // ��� �ð�
        wait_time[name] = time - arrive_t;
        avg_wait += time - arrive_t;

        // ��ȯ �ð�
        turnaround_time[name] = time + burst_t - arrive_t;
        avg_turnaround += time + burst_t -arrive_t;
        
        // ���� �ð�
        burst_time[name] = burst_t;
        
        // �Ϸ� �ð�
        complete_time[name] = time + burst_t;

        // ���� �ð�
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
    // printf("start");
    priority_queue<process, vector<process>,cmpburst> readyQ;

    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    // ���� ������� ���� ��, �켱 ���� ����
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

        // ���� ���μ����� ����

        name = present.name;
        done[done_num] = name;
        done_num += 1;

        
        // ���� ���μ���
        int burst_t = present.burst;
        int arrive_t = present.arrive;
        
        //������ ���� ���� ��Ȳ
        if(arrive_t > time){
            time = arrive_t;
        }

        //��Ʈ ��Ʈ �� �� �߰�
        processing tmp;
        
        tmp.start = time;
        tmp.end = burst_t + time;
        tmp.burst = burst_t;
        processing_time[name].push_back(tmp);
        

    
        // ���μ��� ���� ���� �ð� ���
        wait_time[name] =  time - arrive_t;
        avg_wait +=  (time - arrive_t) ;

        turnaround_time[name] = burst_t + time -arrive_t;
        avg_turnaround += burst_t + time- arrive_t;

        response_time[name] = time - arrive_t;
        avg_response += time - arrive_t;
        
        //���� �ð��� �÷��ش�
        time += burst_t;

        // printf("���� ���μ��� %d\n", name);

        // ������ �Ϸ� �߰�, ���� ���� �͵� �߿��� ���ο� �� ������
        // ������� �ʾҰ� arrive�� �ð����� ���� ���� priority ready queue�� ���� 
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
            
            //������� �ʾҴ��� Ȯ��
            if((Process[i].arrive <= time) && (done_find(done,done_num,Process[i].name) == false) && (isin == false)) {
                // printf("����ť ���μ���: %d\n",Process[i].name);
                //readyqueue�� �����ϰ� done���� ���� �߰�
                readyQ.push(Process[i]);
                                
            }
        }
        // priority ready_queue�� �� �� �߿��� �켱���� ���� �� �̱�
        int next_process = readyQ.top().name;
        

        readyQ.pop();

        // ���� ������ ���ؼ� ���μ������ڸ� ���� �켱������ ���� ���μ��� ������ �ٲ�
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

    // ���� ������ ���� �迭
    sort_arrive(Process);

    // ���� ������ ���ٸ�, ���� �ð��� ª�� ���� �켱
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
    // �ʱ� ���μ��� ����
	present = Slist.front();
	Slist.pop();
	processing pt;
	pt.start = 0;


    while(time<=fin_time){

        

        while (1){
            if (Slist.size() != 0 && Slist.front().arrive == time){
                tmp = Slist.front();
			    Slist.pop();
                if (tmp.burst < present.burst) {

					// ���� ���μ��� ��ü�Ǹ� processing time ���
					pt.end = time;
					pt.burst = time - pt.start;
					if (pt.burst != 0)
						processing_time[present.name].push_back(pt);
					pt.start = time; // ���ο� ���۰� ����

					// ���ο� ���μ��� ������
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
			pt.start = time; // ���ο� ���۰� ����
            if (readyQ.size() != 0) {
				present = readyQ.top();
				readyQ.pop();
		    }
            else if (time != fin_time && readyQ.size() == 0) {
				cout << "SJF �����ٸ� �߰��� ������ ������ϴ�. - �߸��� �Է��Դϴ�.";
				system("pause");
				exit(1);
		    }
		    else break;
        }
        if(response_time[present.name] == -1){
            response_time[present.name] = time - present.arrive;
            avg_response +=(double)response_time[present.name];
        }
        present.burst -= 1; // ���� ó������ ���μ����� burst 1 ����
	    time = time + 1; // �ð��� 1 ����
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
