#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <queue>
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

  bool operator<(const Process &other) const
  {
    return cpu_burst > other.cpu_burst;
  }

  Process(int pid, int arrival_time, vector<int> cpu_burst, vector<int> io_burst)
      : pid(pid), arrival_time(arrival_time), cpu_burst(cpu_burst), io_burst(io_burst) {}
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

vector<Process> read_process_data(string filename)
{
  vector<Process> processes;
  ifstream f(filename);
  if (f.is_open())
  {
    // cout << "File not found";
    int pid, arrival_time, cpu, io, burst;
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
      processes.push_back(Process(pid, arrival_time, cpu_burst, io_burst));
    }
  }
  print_vector(processes);
  return processes;
}

void compute_performance(vector<Process> &processes) {
    int total_wait_time = 0;
    int total_turnaround_time = 0;
    int current_time = 0;
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
  // // Sort processes based on shortest CPU burst time
  // sort(processes.begin(), processes.end(), compareProcesses);
  // print_vector(processes);
  // cout<<"afet"<<endl;
  compute_performance(processes);
  // int num_processes = processes.size();
  // cout << "Number of processes is "<<num_processes<<endl;

  //   // Implement the SJF algorithm
  //   int current_time = 0;
  //   int wait_time = 0;
  //   int turnaround_time = 0;

  //   for (int i = 0; i < num_processes; i++) {
  //     sort(processes.begin(), processes.end(), compareProcess);
  //       current_time = max(current_time, processes[i].arrival_time);
  //       wait_time += current_time - processes[i].arrival_time;
  //       current_time += processes[i].cpu_burst[0];
  //       turnaround_time += current_time - processes[i].arrival_time;
  //       for (int j = 1; j < processes[i].cpu_burst.size(); j++) {
  //           current_time += processes[i].io_burst[j - 1];
  //           current_time += processes[i].cpu_burst[j];
  //           turnaround_time += current_time - processes[i].arrival_time;
  //       }
  //   }
    
  //   // Compute performance measures
  //   double avg_wait_time = (double) wait_time / num_processes;
  //   double avg_turnaround_time = (double) turnaround_time / num_processes;
  //   double throughput = (double) num_processes / current_time;
    
  //   // Print results
  //   cout << "Average waiting time: " << avg_wait_time << endl;
  //   cout << "Average turnaround time: " << avg_turnaround_time << endl;
  //   cout << "Throughput: " << throughput << endl;

  return 0;
}
