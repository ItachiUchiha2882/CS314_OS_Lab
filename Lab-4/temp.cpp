#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <numeric>
using namespace std;

// function to print the vector
template <typename T>
void print_vector(const std::vector<T> &vec)
{
  for (const auto &x : vec)
  {
    cout << x << " ";
  }
  cout << std::endl;
}

// Process class declared here
class Process
{
public:
  int pid, arrival_time;
  vector<int> cpu_burst, io_burst;
  bool is_this_first_time;
  int burst_time; 
  int cpu_burst_time;
  int cpu_burst_size; 
  int io_burst_size;
  int waiting_time; 
  int turnaround_time; 
  int response_time; 
  int completion_time; 
  int remaining_time; 

  bool operator<(const Process &other) const
  {
    return cpu_burst > other.cpu_burst;
  }

  Process(){}

  // Process(int burst_time, vector<int> cpu_burst) : burst_time(burst_time), cpu_burst(cpu_burst) {}
  // Process(vector<int> io_burst, Process(process)) : io_burst(io_burst), Process(process) {}
  
  Process(int pid, int arrival_time, vector<int> cpu_burst, vector<int> io_burst, 
  bool is_this_first_time,
  int burst_time,
  int cpu_burst_time )
      : pid(pid), arrival_time(arrival_time), cpu_burst(cpu_burst), io_burst(io_burst),
      is_this_first_time(is_this_first_time), burst_time (burst_time), cpu_burst_time (cpu_burst_time), 
  cpu_burst_size (cpu_burst_size) {}

  // Process(int pid, int arrival_time, vector<int> cpu_burst, vector<int> io_burst, 
  // bool is_this_first_time,
  // int burst_time,
  // int cpu_burst_time, 
  // int cpu_burst_size, 
  // int io_burst_size, 
  // int waiting_time,
  // int turnaround_time, 
  // int response_time,
  // int completion_time, 
  // int remaining_time)
  //     : pid(pid), arrival_time(arrival_time), cpu_burst(cpu_burst), io_burst(io_burst),
  //     is_this_first_time(is_this_first_time), burst_time (burst_time), cpu_burst_time (cpu_burst_time), 
  // cpu_burst_size (cpu_burst_size), 
  // io_burst_size (io_burst_size), 
  // waiting_time (waiting_time),
  // turnaround_time (turnaround_time), 
  // response_time (response_time),
  // completion_time (completion_time), 
  // remaining_time (remaining_time) {}
  

};

// function which prints parameters after reading the input file.
std::ostream &operator<<(std::ostream &out, const Process &p)
{
  out << "*** Process id: " << p.pid << std::endl;
  out << "Arrival time: " << p.arrival_time << std::endl;
  out << "CPU burst: [ ";
  for (auto burst : p.cpu_burst)
  {
    out << burst << " ";
  }
  out << "]" << std::endl;
  out << "I/O burst: [ ";
  for (auto burst : p.io_burst)
  {
    out << burst << " ";
  }
  out << "]" << std::endl;
  return out;
}

bool compareProcesses(Process a, Process b)
{
  return a.cpu_burst < b.cpu_burst;
}

bool cmp(Process a, Process b)
{
  return a.arrival_time < b.arrival_time;
}

bool cmp2(pair<int, Process> &a, pair<int, Process> &b) {
    if(a.first == b.first) return a.second.pid > b.second.pid;
    return a.first > b.first;
};

struct comparator {
    bool operator()(pair<int, Process> &a, pair<int, Process> &b){
        if(a.first == b.first) return a.second.pid < b.second.pid;
        return a.first < b.first;
    }
};

vector<Process> read_process_data(string filename)
{
  vector<Process> processes;
  ifstream f(filename);
  if (f.is_open())
  {
    // cout << "File not found";
    int pid, arrival_time, cpu, io, burst, burst_time, cpu_burst_time;
    bool is_this_first_time;
    int i = 1, n = 0;
    string rm;
    while (f >> rm)
    {
      if (rm == "<html>")
      {
        continue;
      }
      else if (rm == "<bodyl>")
      {
        continue;
      }
      else if (rm == "<pre>")
      {
        break;
      }
    }
    while (f >> arrival_time)
    {
      pid = i;
      is_this_first_time = true;
      vector<int> cpu_burst, io_burst;
      // while (f >> cpu >> io && io != -1)
      // {
      //   cpu_burst.push_back(cpu);
      //   io_burst.push_back(io);
      // }
      while (f >> burst && burst != -1)
      {
        if (burst != -1)
        {
          if (n % 2 == 0)
          {
            cpu_burst.push_back(burst);
          }
          else
          {
            io_burst.push_back(burst);
          }
          n++;
        }
      }
      n = 0;
      // cout << "pid is " << pid << endl;
      i++;
      reverse(cpu_burst.begin(), cpu_burst.end());
      reverse(io_burst.begin(), io_burst.end());
      burst_time = accumulate(cpu_burst.begin(), cpu_burst.end(), 0);
      cpu_burst_time = burst_time;
      processes.push_back(Process(pid, arrival_time, cpu_burst, io_burst, is_this_first_time, burst_time, cpu_burst_time));
    }
  }
  // print_vector(processes);
  return processes;
}

void compute_performance(vector<Process> &processes) {
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    int current_time = 0;
    int done_process_count = 0;
    // cout<<"insds"<<endl;
    // print_vector(processes);
    priority_queue<Process> pq;
    for (int i = 0; i < processes.size(); i++) {
        Process next_process = processes[i];
        if (!pq.empty()) {
            Process running_process = pq.top();
            pq.pop();
            total_turnaround_time += current_time - running_process.arrival_time;
            total_wait_time += current_time - running_process.cpu_burst[i] - running_process.arrival_time;
            current_time += running_process.cpu_burst[i];
        }
        while (i < processes.size() && processes[i].arrival_time <= current_time) {
            pq.push(processes[i]);
            i++;
        }
        i--;
        if (!pq.empty()) {
            pq.push(next_process);
        }
    }
    int c = 0;
    while (!pq.empty()) {
        Process running_process = pq.top();
        pq.pop();
        total_turnaround_time += current_time - running_process.arrival_time;
        total_wait_time += current_time - running_process.cpu_burst[c] - running_process.arrival_time;
        current_time += running_process.cpu_burst[c];
        c++;
    }
    double avg_turnaround_time = (double)total_turnaround_time / processes.size();
    double avg_wait_time = (double)total_wait_time / processes.size();
    double penalty_ratio = avg_turnaround_time / avg_wait_time;
    double throughput = (double)processes.size() / current_time;
    cout << "Average Turnaround Time: " << avg_turnaround_time << endl;
    cout << "Average Waiting Time: " << avg_wait_time << endl;
    cout << "Penalty Ratio: " << penalty_ratio << endl;
    cout << "Throughput: " << throughput << endl;
}


int main()
{
  // vector<Process> processes = read_process_data("/Users/apple/Desktop/6th sem/CS314 OS lab/Submissions/Lab-4/processes.txt");
  vector<Process> processes = read_process_data("/Users/apple/Desktop/6th sem/CS314 OS lab/Submissions/Lab-4/test_cases/process1.dat");
  
  // int time = 0;
  int count_processes = processes.size();
  cout << "count_processes is " << count_processes << endl;
  // int done_process_count = 0;
  // compute_performance(processes);

  sort(processes.begin(), processes.end(), cmp);

  int n = count_processes;
  // Process cpu_process, io_process;
    bool cpu_idle = true, io_idle = true,not_finished = true;
  auto cmp = cmp2;

  priority_queue<pair<int, Process>, vector<pair<int, Process> >, decltype(cmp)> cpu_queue(cmp);
  priority_queue<pair<int, Process>, vector<pair<int, Process> >, decltype(cmp)> io_queue(cmp);
  
  Process cpu_process = Process();
  Process io_process = Process();
  int time = 0;
    //cpu shoudn't be idle
    int i=0;
    while(not_finished){
        while(i<n && processes[i].arrival_time <= time){
            cpu_queue.push({processes[i].burst_time, processes[i]});
            i++;
        }
        if(cpu_idle){
            if(!cpu_queue.empty()){
                cpu_process = cpu_queue.top().second;
                cpu_queue.pop();
                if(cpu_process.is_this_first_time){
                    for(int k=0;k<n;k++){
                        if(processes[k].pid == cpu_process.pid){
                            processes[k].response_time = time - processes[k].arrival_time;
                            cpu_process.is_this_first_time = false;
                            break;
                        }
                    }
                }
                cpu_idle = false;
                continue;
            }
        }else{
            cpu_process.burst_time--;
            cpu_process.cpu_burst[cpu_process.cpu_burst.size()-1]--;
            if(cpu_process.cpu_burst[cpu_process.cpu_burst.size()-1] == 0){
                if(cpu_process.io_burst.size() == 0){
                    for(int k=0;k<n;k++){
                        if(processes[k].pid == cpu_process.pid){
                            processes[k].completion_time = time+1;
                            break;
                        }
                    }
                }else{
                    cpu_process.cpu_burst.pop_back();
                    io_queue.push({cpu_process.io_burst[cpu_process.io_burst.size()-1], cpu_process});
                    time++;
                    cpu_idle=true;
                    continue;
                }
                cpu_idle = true;
            }
        }
        if(io_idle){
            if(!io_queue.empty()){
                io_process = io_queue.top().second;
                io_queue.pop();
                io_idle = false;
            }
        }else{
            io_process.io_burst[io_process.io_burst.size()-1]--;
            if(io_process.io_burst[io_process.io_burst.size()-1] == 0){
                int x=io_process.cpu_burst.size();
                if(io_process.cpu_burst.size() == 0){
                    //check pid of all process and if it is equal to io_process.pid then set its completion time
                    for(int k=0;k<n;k++){
                        if(processes[k].pid == io_process.pid){
                            processes[k].completion_time = time+1;
                            break;
                        }
                    }

                }else{
                    io_process.io_burst.pop_back();
                    cpu_queue.push({io_process.burst_time, io_process});
                }
                io_idle = true;
            }
        }
        time++;
       if(cpu_queue.empty() && io_queue.empty() && cpu_idle && io_idle) not_finished = false;
    }
  
  cout<<"hehe";
  return 0;
}
