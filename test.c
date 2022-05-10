typedef struct
{
	char *name;//이름
    int arrive;//도착
    int service;//서비스
    int priority;//우선순위
    int time;//실행시간
} process;

int num;
int quantum = 1;

int* burst_time; // bursttime 배열
int* complete_time; // completetime 배열
int* wait_time; // waiting time 배열
int* turnaround_time; //turnaround time 배열

int main(){
    int num;
    scanf("개수를 입력하시오: %d",&num);

    process *Process;
	Process = (process *)malloc( sizeof(process) * num);

}