#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    int arrivalTime;
    int cpuBurst;
    int remainingTime;
};

bool sortByRemainingTime(const Process &a, const Process &b) {
    return a.remainingTime < b.remainingTime;
}

int main() {
    vector<Process> processes;

    // Read process data from file
    ifstream file("processes.txt");
    int arrivalTime, cpuBurst;
    while (file >> arrivalTime >> cpuBurst) {
        processes.push_back({arrivalTime, cpuBurst, cpuBurst});
        int ioBurst;
        while (file >> ioBurst && ioBurst != -1) {
            // Read I/O burst
        }
    }

    int currentTime = 0, completed = 0;
    while (completed < processes.size()) {
        sort(processes.begin(), processes.end(), sortByRemainingTime);

        // Check for new process arrival
        while (processes.size() > 0 && processes[0].arrivalTime <= currentTime) {
            Process p = processes[0];
            processes.erase(processes.begin());
            // Add process to queue
        }

        if (processes.size() == 0) {
            currentTime++;
            continue;
        }

        // Assign CPU to process
        Process p = processes[0];
        processes.erase(processes.begin());
        p.remainingTime--;
        currentTime++;

        if (p.remainingTime > 0) {
            // Add process back to queue
        } else {
            completed++;
        }
    }

    // Compute and display performance measures

    return 0;
}
