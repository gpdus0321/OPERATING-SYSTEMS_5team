#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <queue>
#include <stdlib.h>
using namespace std;

typedef struct process {
    int name;
    int arrive;
    int burst;
    int priority;
}process;

typedef struct processing {
    int start;
    int end;
    int burst;
}processing;

int num = 0;
int TQ = 0;

vector<processing>* processing_time; // processingtime 배열
int* burst_time; // bursttime 배열
int* complete_time; // completetime 배열
int* wait_time; // waiting time 배열
int* turnaround_time; //turnaround time 배열
int* response_time;


//////// RR은 도착시간 순서이긴한데 TQ에 맞춰서 계속 바뀔 수 있다.

//queue에 넣어야지
//도착순으로 정렬 됬으니까
//이제 queue 넣어
//근데 이제 TQ를 생각하면서
//만약 TQ가 2라고 한다면
//queue에 process[0]번째 2번 insert 해주고, burst -= 2 해주고,O
//process[1]번째 2번 insert 해주고, burst -= 2 해주고, O
//만약 burst에 값이 0이라면, 거기는 이제 queue에 insert안해주고, 다음꺼를 찾는다. O
//만약 burst 값이 1 이라고 하면, queue에 한번만 insert해주어야한다. O

//이렇게 반복인가?

//RR은 서비스 타임, TQ, 도착시간만 있으면됨
//TQ시간 만큼 빼야지,,,,,

//각 프로세스별 대기시간 및 평균 대기시간, 각 프로세스별 반환시간 및 평균 반환시간, 각 프로세스별 응답시간 및 평균 응답시간
//**********redyQ 필요함!!!!!!
void RR(process* Process);
void Priority(process* Process);
void print_(double avr_wait, double avr_turnaround, double avr_response);
void sort_arrive(process* Process);
void sort_priority(process* Process, int i, int temp);


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
    process ing;
    processing pt;

    double avr_wait = 0.0, avr_turnaround = 0.0, avr_response = 0.0;
    queue<process> Plist;
    queue<process> readyQ;

    for (i = 0; i < num; i++)
        processing_time[i].clear();


    for (i = 0; i < num; i++) {
        Plist.push(Process[i]);
        sum += Process[i].burst;
    }
    i = 0;
    while (i < sum) {

        while (1) {
            if (Plist.size() != 0 && Plist.front().arrive <= i) {
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
                if (Plist.size() != 0 && Plist.front().arrive <= i + TQ) {
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
                if (Plist.size() != 0 && Plist.front().arrive <= i + burst_) {
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
        response_time[name_] = processing_time[name_][0].start - arrive_;
        avr_response += (double)response_time[name_];
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
    //도착시간 순으로 정렬
    sort_arrive(Process);

    int i = 0, j = 0;
    processing pt;
    pt.end = 0;
    double avr_wait = 0.0, avr_turnaround = 0.0, avr_response = 0.0;

    //프로세스 타임 초기화
    for (i = 0; i < num; i++)
        processing_time[i].clear();

    for (i = 0; i < num; i++) {
        j = i;
        while (Process[j].arrive <= pt.end) {
            if(j>=num) break;
            j++;
        }
        sort_priority(Process, i, j);

        int name_ = Process[i].name;
        int burst_ = Process[i].burst;
        int arrive_ = Process[i].arrive;

        pt.start = pt.end;
        pt.end = burst_ + pt.end;
        pt.burst = burst_;
        processing_time[name_].push_back(pt);

        wait_time[name_] = pt.start - arrive_;
        avr_wait += (double)wait_time[name_];

        turnaround_time[name_] = pt.end - arrive_;
        avr_turnaround += (double)turnaround_time[name_];

        response_time[name_] = pt.start - arrive_;
        avr_response += (double)response_time[name_];
    }

    avr_wait /= (double)num;
    avr_turnaround /= (double)num;
    avr_response /= (double)num;
    print_(avr_wait, avr_turnaround, avr_response);

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
        for (j = 0; j < num - 1 - i; j++) {
            if (Process[j].arrive > Process[j + 1].arrive) {
                process temp = Process[j + 1];
                Process[j + 1] = Process[j];
                Process[j] = temp;
            }
        }
    }
}

void sort_priority(process* Process, int i, int temp) {
    int j, x;
    process tmp, min;
    int index = 0;
    for (x = i; x < temp - 1; x++) {
        min.priority = 100;
        for (j = x; j < temp; j++) {
            if (min.priority > Process[j].priority) {
                min = Process[j];
                index = j;
            }
        }
        tmp = Process[x];
        Process[x] = Process[index];
        Process[index] = tmp;
    }
}