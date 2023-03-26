#include <iostream>
#include <vector>
#include <unordered_map>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    cout << "Please enter the correct number of arguments." << endl;
    cout << "Usage: ./200010004_random <num_pages> <num_frames> <num_blocks> <filename>" << endl;
    return 1;
  }

  int num_pages = stoi(argv[1]);  // Total number of pages in the system
  int num_frames = stoi(argv[2]); // Number of frames in main memory
  int num_blocks = stoi(argv[3]); // Number of disk blocks for swap space
  string filename = argv[4];      // Filename containing sequence of page requests

  // int swap_space = 60;
  // if(num_blocks > swap_space){
  //   cout << "Error: num_blocks cannot be greater than 60" << endl;
  //   return 1;
  // }

  unordered_map<int, int> in_memory; // Create page table with no mappings

  // Create vector to store frame usage information
  vector<int> frameUsage(num_frames, -1); // Initialize all frames to -1

  int pageFaults = 0, page_hits = 0;

  srand(time(0)); // Seed random number generator with current time

  // Open input file
  ifstream inputFile(filename);
  if (!inputFile.is_open())
  {
    cout << "Error: could not open the file " << filename << endl;
    return 1;
  }

  // Read in page requests and simulate memory access
  int page, pageCount = 0;
  while (inputFile >> page)
  {
    // Check if page is in memory
    pageCount++;
    if (in_memory.find(page) != in_memory.end())
    {
      page_hits++;
    }
    else
    {
      // Page fault, select victim frame using random policy
      int victim_frame = rand() % num_frames;
      // Check if victim frame is currently in use
      if (frameUsage[victim_frame] != -1)
      {
        // Update page table to remove mapping for victim frame
        int victim_page = frameUsage[victim_frame];
        in_memory.erase(victim_page);
      }

      // Update page table and frame usage vectors with new mapping
      in_memory[page] = victim_frame;
      frameUsage[victim_frame] = page;

      // Increment page fault count
      pageFaults++;
    }
  }

  // Output results
  // cout << "Page count: " << pageCount << endl;
  cout << "** Random Page Replacement Algorithm **" << endl;
  cout << "Total number of page faults: " << pageFaults << endl;
  cout << "Total number of page hits: " << page_hits << endl;

  return 0;
}
