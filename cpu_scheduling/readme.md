Read data from a file "input.dat"
The first line is <number of processes>
Second line is <quantum value>
Third (and next N-1 subsequent lines, N is number of processes>
<pid> <priority> <arrival time> P <processor burst> I <input burst> ......O <output burst> -1

So each process shall consist of time intervals needed to be spent on processor (Processor Burst) and Input or output device (Input burst, output burst respectively). Two processor bursts are separated by one input or one output burst. Last burst shall be processor burst. -1 is terminator.
So
1 3 6 P 4 I 3 P 7 O 6 P 2 -1
means that process with Id=1, priority = 3 and arrival time =6, requires processor for 4 quanta, input for 3, again processor for 7, output for 6 and processor for 2. It may be noted that if a device (CPU/Input/Output) is busy, the process has to wait in the device's queue. Once the process gets scheduled on the device, it shall occupy the device for respective burst time or till its gets context switched. For Input/Output device, FCFS scheduling shall be used. The system needs to be simulated for following CPU scheduling methods
(1) SRTF (Shortest Remaining Time First - remaining time to be considered till next I/O burst or completion whichever is nearer)
(2) SJF (Shortest Job First)

the output should be
<Name of scheduling algorithm>
(1) Turnaround time of each process , Average Turnaround Time
(2) Response time of each process , Average Response Time
(3) Waiting time of each process , Average Waiting Time
