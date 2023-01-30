#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename T>
void print_vector(const std::vector<T> &vec)
{
  for (const auto &x : vec){
    cout << x << " ";
  }
  cout << std::endl;
}

class Process
{
public:
  int pid, arrival_time;
  vector<int> cpu_burst, io_burst;
  Process(int pid, int arrival_time, vector<int> cpu_burst, vector<int> io_burst)
      : pid(pid), arrival_time(arrival_time), cpu_burst(cpu_burst), io_burst(io_burst) {}
};

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

vector<Process> read_process_data(string filename)
{
  vector<Process> processes;
  ifstream f(filename);
  if (f.is_open())
  {
    // cout << "File not found";
    int pid, arrival_time, cpu, io;
    int i = 1;
    while (f >> arrival_time)
    {
      pid = i;
      vector<int> cpu_burst, io_burst;
      // while (f >> cpu >> io && cpu != -1)
      while (f >> cpu >> io && io != -1)
      {
        cpu_burst.push_back(cpu);
        io_burst.push_back(io);
      }
      cout << "pid is " << pid << endl;
      i++;
      processes.push_back(Process(pid, arrival_time, cpu_burst, io_burst));
    }
  }
  print_vector(processes);
  return processes;
}

pair<double, double> sjf_scheduler(vector<Process> processes)
{
  int current_time = 0;
  double waiting_time = 0;
  double turnaround_time = 0;
  double response_time = 0;
  // for (const Process& process : processes) {
  //   if (current_time < process.arrival_time) {
  //     current_time = process.arrival_time;
  //   }
  //   waiting_time += current_time - process.arrival_time;
  //   current_time += accumulate(process.cpu_burst.begin(), process.cpu_burst.end(), 0)
  //                + accumulate(process.io_burst.begin(), process.io_burst.end(), 0);
  //   turnaround_time += current_time - process.arrival_time;
  // }
  for (const Process &process : processes)
  {
    if (current_time < process.arrival_time)
    {
      current_time = process.arrival_time;
    }
    int cpu_burst_sum = 0;
    for (int burst : process.cpu_burst)
    {
      cpu_burst_sum += burst;
    }
    int io_burst_sum = 0;
    for (int burst : process.io_burst)
    {
      io_burst_sum += burst;
    }
    waiting_time += current_time - process.arrival_time;
    current_time += cpu_burst_sum + io_burst_sum;
    turnaround_time += current_time - process.arrival_time;
    response_time += process.arrival_time;
  }
  return make_pair(waiting_time / processes.size(), turnaround_time / processes.size());
}

int main()
{
  vector<Process> processes = read_process_data("/Users/apple/Desktop/6th sem/CS314 OS lab/Submissions/Lab-4/processes.txt");
  const auto [waiting_time, turnaround_time] = sjf_scheduler(processes);
  // print_vector(processes);
  cout << "Average waiting time: " << waiting_time << endl;
  cout << "Average turnaround time: " << turnaround_time << endl;
  return 0;
}