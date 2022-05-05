#include<iostream>
#include<deque>
#include<queue>
#include<utility>
#include<vector>
#include<fstream>
#include<string>
#include<pthread.h> //for threading
#include<unistd.h>  //for sleep
#include<semaphore.h>// mutual exclusion
#include"pcb.h"

using namespace std;

sem_t semaphore;

//time elapsed
int clock =0;

int quanta =2;
int terminated=0;
int num_process=0;

// states of process
vector<pcb>ready_queue;
queue<pcb>input_dev;
queue<pcb>output_dev;
vector<pcb>all_process;

void processor(pcb process,int quanta)
{

}

void allocating_queue(pcb temp)
{
    cout << "Allocating process with pid: " << temp.get_pid() << endl;
    cout << "PID: "<<temp.get_pid()  << " burst left:" << temp.get_process_left()<< endl;

    if(temp.empty() == 1)
    {
        cout << "Terminating process with PID : " << temp.get_pid() << endl;
        cout << "TURN AROUND TIME OF THE PROCESS IS " << clock - temp.get_arrival_time() << endl;
        terminated++;
    }
    else{
        pair<char,int>pro = temp.check_burst(); // queuing in the proper queue for next burst of the process

        sem_wait(&semaphore);

        if(pro.first == 'P') {
            cout << "process with pid: " << temp.get_pid() << " queued to READY QUEUE" << endl;
            ready_queue.push_back(temp);
        }
        else if(pro.first == 'O') {
            cout << "process with pid: " << temp.get_pid() << " queued to OUTPUT DEV" << endl;
            output_dev.push(temp);
        }
        else if(pro.first == 'I') {
            cout << "process with pid: " << temp.get_pid() << " queued to INPUT DEV" << endl;
            input_dev.push(temp);
        }

        sem_post(&semaphore);
    }
}

void* input(void* arg)
{
    cout <<"--> Input device is ready" << endl;
    while( terminated < num_process )
    {
        if(input_dev.empty())
        {
            // cout << "Waiting for input" << endl;
            sleep(1);
            continue;
        }
        else{
            cout <<"Processing input" << endl;
        }

        pcb temp= input_dev.front();
        input_dev.pop();
        

        pair<char,int>input_pro = temp.get_burst();
        cout << "Input Started of PID" << temp.get_pid() << "timeStamp: " << clock << endl;

        while(input_pro.second--)
        {
            // cout << "taking input PID:" << temp.get_pid() << "timeStamp: " << clock <<endl;
            sleep(1);
        }
        cout << "Input completed of PID" << temp.get_pid() << "burst Left : " << temp.get_process_left() << "timeStamp: " << clock << endl;

        allocating_queue(temp);
        // if(temp.empty() == 1) continue;
        // else{
        //     input_pro = temp.check_burst(); // queuing in the proper queue for next burst of the process

        //     if(input_pro.first == 'P') ready_queue.push_back(temp);
        //     if(input_pro.first == 'O') output_dev.push(temp);
        //     if(input_pro.first == 'I') input_dev.push(temp);
        // }

    }
    cout << "Closing Input Device" << endl;
    return 0;
}

void* time_counter(void *arg)
{
    while(true)
    {
        sleep(1);
        clock++;
    }

    return 0;
}

void* output(void* arg)
{
    cout <<"--> Output device is ready" << endl;
    while(terminated < num_process)
    {
        if(output_dev.empty()) continue;
        else{
            cout << "Processing output"<< endl;
        }
        pcb temp= output_dev.front();
        output_dev.pop();

        pair<char,int>output_pro = temp.get_burst();
        cout << "Output started of PID" << temp.get_pid() << "timeStamp: " << clock << endl;
        while(output_pro.second--)
        {
            // cout << "Outputting PID:" << temp.get_pid() << "timeStamp: " << clock <<endl;
            sleep(1);
        }
        cout << "Output completed of PID" << temp.get_pid() << "timeStamp: " << clock << endl;


        allocating_queue(temp);
        // if(temp.empty() == 1) continue;
        // else{
        //     output_pro = temp.check_burst(); // queuing in the proper queue for next burst of the process

        //     if(output_pro.first == 'P') ready_queue.push_back(temp);
        //     if(output_pro.first == 'O') output_dev.push(temp);
        //     if(output_pro.first == 'I') input_dev.push(temp);
        // }

    }

    return 0;
}

void* creater(void *arg)
{
    while(all_process.empty() == 0)
    {
        for(int x=0;x<all_process.size();x++)
        {
            if(all_process.at(x).get_arrival_time() < clock)
            {
                ready_queue.push_back(all_process.at(x));
                cout << "Process with PID : " << all_process.at(x).get_pid() << " has arrived in the ready queue at " << clock << endl;
                all_process.erase(all_process.begin()+ x);
                x--;
            }
        }
    }

    return 0;
}

void srtf() //shortest remaining time first 
{
    cout << "\n\n\n" ;
    cout << "starting SRTF at clock value :" << clock << endl;

    pthread_t tid_input,tid_output,tid_clock,tid_create;
    pthread_create(&tid_input,0,input,0);
    pthread_create(&tid_output,0,output,0);
    pthread_create(&tid_clock,0,time_counter,0);
    pthread_create(&tid_create,0,creater,0);


    while(terminated < num_process)
    {
        for(int x=0;x<ready_queue.size();x++)
        {
            pcb temp = ready_queue.at(x);

            if(temp.check_burst().first == 'P') continue;

            allocating_queue(temp);
            ready_queue.erase(ready_queue.begin()+x);
            cout << "Size of ready queue" << ready_queue.size() << endl;  
            
            x--;
        }

        if( ready_queue.size() == 0 )
        {
            cout << "empty ready queue " << endl;
            sleep(1);
            continue;
        }

        pcb srt; // shortest remaining time
        bool flag =0;
        int ind=-1;

        for(int x=0;x<ready_queue.size();x++)
        {
            pcb temp = ready_queue.at(x);

            if(temp.check_burst().first == 'P')
            {
                if(flag == 0) {
                    srt = temp;
                    ind = x;
                    flag = 1;
                }
                else if(temp.check_burst().second < srt.check_burst().second)
                {
                    // ready_queue.push_back(srt);
                    srt= temp;
                    ind = x;
                }
            }    
        }
        sem_wait(&semaphore);
        ready_queue.erase(ready_queue.begin()+ind);
        sem_post(&semaphore);

        pair<char,int> proc = srt.get_burst();
        int time_in_processor = min(quanta,proc.second);
        cout << "Selected PID: " << srt.get_pid() << " burst time: " << proc.second << endl;
        while(time_in_processor--)
        {
            cout << "Executing process with PID : " << srt.get_pid() << " time : " << clock << endl;
            sleep(1);
        }

        
        if(quanta >= proc.second)  // process is completed
        {
            allocating_queue(srt);
        }
        else  // premption
        {
            srt.edit_burst(proc.first ,proc.second - quanta);
            ready_queue.push_back(srt);
        }

        
    }

    pthread_join(tid_input,0);
    pthread_join(tid_output,0);

    cout << "*********All processed completed at : " << clock << endl;

    // cout << "Turnaround time of each processes" << 
}



int main()
{


    ifstream newfile;
    newfile.open("input.dat",ios::in); 
    
    string str;
    int count{1};

    sem_init(&semaphore,0,1);

    while ( getline(newfile, str) )
    {
        cout << str << endl;
        switch(count)
        {
            case 1:
                num_process = stoi(str);
                break;

            case 2:
                quanta = stoi(str);
                break;

            default:
                // cout << count << ":" << str << endl;
                pcb* p = new pcb(str.at(0)-48,str.at(2)-48,str.at(4)-48);
                for(int x=6;x<str.size();x+=2)
                {
                    if(x== -1) break;
                    if(str.at(x)>65)
                    {
                        // cout << "adding " << str.at(x) << ":" << str.at(x+2) << endl;
                        p->add_burst(str.at(x), str.at(x+2)-48 );
                    }
                }
                
                p->display();
                all_process.push_back(*p);
                cout <<" pcb added" << endl;
                break;
        }

        count++;
    }


    cout <<"inp complelted" << endl;

    srtf();
    cout << "no. of process" << num_process << endl;
    // cout << "quanta " << quanta << endl;  
    // cout << "Size of ready queue" << ready_queue.size() << endl;  

}