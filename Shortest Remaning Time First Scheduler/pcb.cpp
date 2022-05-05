#include<iostream>
// #include<algorithm>
#include<deque>
#include<utility>
#include"pcb.h"

using namespace std;


pcb::pcb(){};
void pcb::display()
{
    cout << "pid:" << pid << endl;
    cout << "priority" << priority << endl;
    cout << "arrival time:" << arrival_time <<endl;
}
pcb::pcb(int pid,int priority,int arrival_time)
{
    this->pid = pid;
    this->priority = priority;
    this->arrival_time = arrival_time;
}

int pcb::get_pid()
{
    return pid;
}
void pcb::add_burst(char c,int time)
{
    burst.push_back({c,time});
}

void pcb::edit_burst(char c,int time)
{
    burst.push_front({c,time});
}

int pcb::get_process_left()
{
    return burst.size();
}

pair<char,int> pcb::check_burst()
{
    return burst.front();
}

bool pcb::empty()
{
    return burst.empty();
}

pair<char,int> pcb::get_burst()
{
    pair<char,int>temp = burst.front();
    cout << "PID: " << pid << " left burst:" << burst.size() << endl;
    burst.pop_front();

    return temp;
}
