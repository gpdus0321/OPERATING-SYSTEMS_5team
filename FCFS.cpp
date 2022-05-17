#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <list>
#include <queue>
using namespace std;

typedef struct
{
	int name;//
    int arrive;//
    int service;//
    int priority;//
    int tq;//tq
} process;



int num;
int quantum = 1;
int fin_time = 0;

int* burst_time; // bursttime 
int* complete_time; // completetime 
int* wait_time; // waiting time 
int* turnaround_time; //turnaround time



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



bool arrin(int num, int* arr, int arr_num)
{
    for(int i=0; i<arr_num; i++){
        if(arr[i] == num){
            return true;
        } 
    }
    return false;
}


void FCFS( process *Process, int num ){
    
    
    burst_time = new int[num];
    complete_time = new int[num];
    wait_time = new int[num];
    turnaround_time = new int[num];



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
    
    //異쒕젰 媛? 
    int avg_wait = 0;
    int avg_turnaround = 0;

    for(int i = 0; i< num; i++){

        int burst_t = Process[i].service;
        int arrive_t = Process[i].arrive;
       
        while(time < arrive_t){
            time += 1;
        }

        
        wait_time[i] = time - arrive_t;
        avg_wait += time - arrive_t;

        turnaround_time[i] = time + burst_t;
        avg_turnaround += time + burst_t;
        
        
        burst_time[i] = burst_t;

        complete_time[i] = time + burst_t;

        time += burst_t; 
        fin_time = time;
    }

    avg_wait /= num;
    avg_turnaround /= num;

    cout << "avg_wait:" << avg_wait << endl;
    cout << "avg_turnaroud" << avg_turnaround << endl;

}



void SJF( process *Process, int num ){
    

    burst_time = new int[num];
    complete_time = new int[num];
    wait_time = new int[num];
    turnaround_time = new int[num];
    
    // ready queue 
    queue<process>readyQ;

   
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
    int* done = new int[num];

    int process_num = 0;

    while(time < fin_time){

        int burst_t = Process[process_num].service;
        int arrive_t = Process[process_num].service;
        
        while(time < arrive_t){
            time += 1;
        }

        wait_time[process_num] = arrive_t- time;
        turnaround_time[process_num] = burst_t + time;
        
        

        for(int i = 0; i< num; i++){
            if((Process[i].arrive <= time + burst_t) && (!arrin(i, done, num)) ){
                readyQ.push(Process[i]);
            }
        }
        
        int min_burst_time = readyQ.front().service;
        int min_burst_num = 0;
        for(int i = 0; i< readyQ.size(); i++){
            process temp = readyQ.front();
            
        }

    }


}