#pragma once

#include<deque>
#include<iostream>
#include<utility>

using namespace std;


class pcb{
    int pid;
    int priority;
    int arrival_time;
    int termination_time;
    
    deque< pair<char,int> >burst;

public:
    pcb();
    pcb(int pid,int priority,int arrival_time);
    int get_pid();
    void display();
    int get_process_left();
    bool empty();
    void add_burst(char c,int time);
    void edit_burst(char c,int time);
    void set_termination_time(int time);
    int get_arrival_time();
    pair<char,int> get_burst();
    pair<char,int> check_burst();
};