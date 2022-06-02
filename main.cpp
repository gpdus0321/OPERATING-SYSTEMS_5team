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

typedef struct processing
{
    int start;
    int end;
    int burst;
} processing;

// ����ð� ����
struct cmpburst
{
    bool operator()(process a, process b)
    {
        if (a.burst == b.burst)
        {
            return a.arrive > b.arrive;
        }
        return a.burst > b.burst;
    }
};

int num = 0;
int TQ = 0;
int fin_time = 0;

int *burst_time;                     // bursttime
int *complete_time;                  // completetime
int *wait_time;                      // waiting time
int *turnaround_time;                // turnaround time
int *response_time;                  // response time
vector<processing> *processing_time; //�� ���μ��� ���� ��Ʈ��Ʈ�� ��

void RR(process *Process);
void Priority(process *Process);
void print_(double avr_wait, double avr_turnaround, double avr_response);
void sort_priority(process *Process, int i, int temp);
void sort_arrive(process *Process);
void FCFS(process *Process);
bool done_find(int done[], int done_len, int num);
void SJF(process *Process);
void SRTF(process *Process);

int main()
{
    int name;
    int arrive, burst, priority;

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

    for (int i = 0; i < num; i++)
    {
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
    SRTF(Process);
    RR(Process);
    Priority(Process);

    delete[] wait_time;
    delete[] processing_time;
    delete[] complete_time;
    delete[] turnaround_time;
    delete[] burst_time;
    delete[] response_time;

    return 0;
}

void sort_arrive(process *Process)
{
    int i, j;

    for (i = 0; i < num - 1; i++)
    {
        for (j = 0; j < num - 1 - i; j++)
        {
            if (Process[j].arrive > Process[j + 1].arrive)
            {
                process temp = Process[j + 1];
                Process[j + 1] = Process[j];
                Process[j] = temp;
            }
        }
    }
}

void FCFS(process *Process)
{
    sort_arrive(Process);

    int time = 0;
    processing tmp;

    double avg_wait = 0.0;
    double avg_turnaround = 0.0;
    double avg_response = 0.0;

    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    for (int i = 0; i < num; i++)
    {

        int burst_t = Process[i].burst;
        int arrive_t = Process[i].arrive;

        // ��Ʈ ��Ʈ�� �� �ۼ�
        tmp.burst = burst_t;
        tmp.start = time;
        tmp.end = burst_t + time;
        int name = Process[i].name;
        processing_time[name].push_back(tmp);

        while (time < arrive_t)
        {
            time += 1;
        }

        // ��� �ð�
        wait_time[name] = time - arrive_t;
        avg_wait += time - arrive_t;

        // ��ȯ �ð�
        turnaround_time[name] = time + burst_t - arrive_t;
        avg_turnaround += time + burst_t - arrive_t;

        // ���� �ð�
        burst_time[name] = burst_t;

        // �Ϸ� �ð�
        complete_time[name] = time + burst_t;

        // ���� �ð�
        response_time[name] = time - arrive_t;
        avg_response += time - arrive_t;

        time += burst_t;
        fin_time = time;
    }

    avg_wait /= (double)num;
    avg_turnaround /= (double)num;
    avg_response /= (double)num;
    print_(avg_wait, avg_turnaround, avg_response);
}

bool done_find(int done[], int done_len, int num)
{
    for (int i = 0; i < done_len; i++)
    {
        if (done[i] == num)
        {
            return true;
        }
    }
    return false;
}

void SJF(process *Process)
{

    // ready queue
    priority_queue<process, vector<process>, cmpburst> readyQ;

    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    // ���� ������� ���� ��, �켱 ���� ����
    sort_arrive(Process);
    for (int i = 0; i < num - 1; i++)
    {
        for (int j = 0; j < num - i; j++)
        {
            if (Process[j].arrive == Process[j + 1].arrive && Process[j].burst > Process[j + 1].burst)
            {
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

    while (time < fin_time)
    {

        // ���� ���μ����� ����

        name = present.name;
        done[done_num] = name;
        done_num += 1;

        // ���� ���μ���
        int burst_t = present.burst;
        int arrive_t = present.arrive;

        //������ ���� ���� ��Ȳ
        if (arrive_t > time)
        {
            time = arrive_t;
        }

        //��Ʈ ��Ʈ �� �� �߰�
        processing tmp;

        tmp.start = time;
        tmp.end = burst_t + time;
        tmp.burst = burst_t;
        processing_time[name].push_back(tmp);

        // ���μ��� ���� ���� �ð� ���
        wait_time[name] = time - arrive_t;
        avg_wait += (time - arrive_t);

        turnaround_time[name] = burst_t + time - arrive_t;
        avg_turnaround += burst_t + time - arrive_t;

        response_time[name] = time - arrive_t;
        avg_response += time - arrive_t;

        //���� �ð��� �÷��ش�
        time += burst_t;

        // ������ �Ϸ� �߰�, ���� ���� �͵� �߿��� ���ο� �� ������
        // ������� �ʾҰ� arrive�� �ð����� ���� ���� priority ready queue�� ����
        for (int i = 0; i < num; i++)
        {
            priority_queue<process, vector<process>, cmpburst> readyQ2;
            readyQ2 = readyQ;
            bool isin = false;
            for (int j = 0; j < readyQ.size(); j++)
            {
                process tmp = readyQ2.top();
                readyQ2.pop();
                if (tmp.name == Process[i].name)
                {
                    isin = true;
                }
            }

            //������� �ʾҴ��� Ȯ��
            if ((Process[i].arrive <= time) && (done_find(done, done_num, Process[i].name) == false) && (isin == false))
            {
                // readyqueue�� �����ϰ� done���� ���� �߰�
                readyQ.push(Process[i]);
            }
        }
        // priority ready_queue�� �� �� �߿��� �켱���� ���� �� �̱�
        int next_process = readyQ.top().name;

        readyQ.pop();

        // ���� ������ ���ؼ� ���μ������ڸ� ���� �켱������ ���� ���μ��� ������ �ٲ�
        for (int i = 0; i < num; i++)
        {
            if (Process[i].name == next_process)
            {
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

void SRTF(process *Process)
{

    priority_queue<process, vector<process>, cmpburst> readyQ;

    int burst[num];
    for (int i = 0; i < num; i++)
    {
        burst[i] = Process[i].burst;
    }

    for (int i = 0; i < num; i++)
        processing_time[i].clear();

    // ���� ������ ���� �迭
    sort_arrive(Process);

    // ���� ������ ���ٸ�, ���� �ð��� ª�� ���� �켱
    for (int i = 0; i < num - 1; i++)
    {
        for (int j = 0; j < num - i; j++)
        {
            if (Process[j].arrive == Process[j + 1].arrive && Process[j].burst > Process[j + 1].burst)
            {
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

    for (int j = 0; j < num; j++)
    {
        Slist.push(Process[j]);
        total += Process[j].burst;
    }
    process present, tmp;
    // �ʱ� ���μ��� ����
    present = Slist.front();
    Slist.pop();
    processing pt;
    pt.start = 0;

    while (time <= fin_time)
    {

        while (1)
        {
            if (Slist.size() != 0 && Slist.front().arrive == time)
            {
                tmp = Slist.front();
                Slist.pop();
                if (tmp.burst < present.burst)
                {

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
                else
                    readyQ.push(tmp);
            }
            else
                break;
        }
        if (present.burst == 0)
        {
            int name = present.name;
            int burst = present.burst;
            int arrive = present.arrive;
            complete = time;

            if (done_find(done, done_num, name) == false)
            {
                done[done_num] = name;
                done_num += 1;
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
            pt.start = time; // ���ο� ���۰� ����
            if (readyQ.size() != 0)
            {
                present = readyQ.top();
                readyQ.pop();
            }
            else if (time != fin_time && readyQ.size() == 0)
            {
                cout << "SJF �����ٸ� �߰��� ������ ������ϴ�. - �߸��� �Է��Դϴ�.";
                system("pause");
                exit(1);
            }
            else
                break;
        }
        present.burst -= 1; // ���� ó������ ���μ����� burst 1 ����
        time = time + 1;    // �ð��� 1 ����
    }
    avg_turnaround /= (double)num;
    avg_wait /= (double)num;
    avg_response /= (double)num;

    for (int i = 0; i < num; i++)
    {
        Process[i].burst = burst[i];
    }

    print_(avg_wait, avg_turnaround, avg_response);
}

void RR(process *Process)
{
    sort_arrive(Process);

    int i, sum = 0, complete = 0, turnaround = 0, wait = 0;
    process ing;
    processing pt;

    double avr_wait = 0.0, avr_turnaround = 0.0, avr_response = 0.0;
    queue<process> Plist;
    queue<process> readyQ;

    for (i = 0; i < num; i++)
        processing_time[i].clear();

    for (i = 0; i < num; i++)
    {
        Plist.push(Process[i]);
        sum += Process[i].burst;
    }
    i = 0;
    while (i < sum)
    {

        while (1)
        {
            if (Plist.size() != 0 && Plist.front().arrive <= i)
            {
                readyQ.push(Plist.front());
                Plist.pop();
            }
            else
                break;
        }

        ing = readyQ.front();
        readyQ.pop();

        int name_ = ing.name;
        int burst_ = ing.burst;
        int arrive_ = ing.arrive;
        pt.start = i;

        //큰경우
        if (burst_ > TQ)
        {
            while (1)
            {
                if (Plist.size() != 0 && Plist.front().arrive <= i + TQ)
                {
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
        else
        {
            while (1)
            {
                if (Plist.size() != 0 && Plist.front().arrive <= i + burst_)
                {
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
void Priority(process *Process)
{
    //도착시간 순으로 정렬
    sort_arrive(Process);

    int i = 0, j = 0;
    processing pt;
    pt.end = 0;
    double avr_wait = 0.0, avr_turnaround = 0.0, avr_response = 0.0;

    //프로세스 타임 초기화
    for (i = 0; i < num; i++)
        processing_time[i].clear();

    for (i = 0; i < num; i++)
    {
        j = i;
        while (Process[j].arrive <= pt.end)
        {
            if (j >= num)
                break;
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

void sort_priority(process *Process, int i, int temp)
{
    int j, x;
    process tmp, min;
    int index = 0;
    for (x = i; x < temp - 1; x++)
    {
        min.priority = 100;
        for (j = x; j < temp; j++)
        {
            if (min.priority > Process[j].priority)
            {
                min = Process[j];
                index = j;
            }
        }
        tmp = Process[x];
        Process[x] = Process[index];
        Process[index] = tmp;
    }
}

void print_(double avr_wait, double avr_turnaround, double avr_response)
{
    int i, j;

    cout << endl
         << "간트 차트" << endl;
    //간트차트는 start 오름차순 정렬해서 죽 뽑으면 될듯.

    for (int i = 0; i < num; i++)
    {
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