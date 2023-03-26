#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
// #include <unordered_map>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    cout << "Please enter the correct number of arguments." << endl;
    cout << "Usage: ./200010004_fifo <num_pages> <num_frames> <num_blocks> <filename>" << endl;
    return 1;
  }
  // take input from command line
  int num_pages = stoi(argv[1]);  // Total number of pages in the system
  int num_frames = stoi(argv[2]); // Number of frames in main memory
  int num_blocks = stoi(argv[3]); // Number of disk blocks for swap space
  string filename = argv[4];      // Filename containing sequence of page requests

  // int swap_space = 60;
  // if(num_blocks > swap_space){
  //   cout << "Error: num_blocks cannot be greater than 60" << endl;
  //   return 1;
  // }

  // Open the file and read page references into a vector
  vector<int> pageReferences;
  int page, pageCount = 0;
  ifstream inputFile(filename);
  if (!inputFile.is_open())
  {
    cout << "Error: could not open the file " << filename << endl;
    return 1;
  }
  // pageReferences.push_back(0);
  while (inputFile >> page)
  {
    pageCount++;
    pageReferences.push_back(page);
  }
  // cout << "Page count: " << pageCount << endl;
  inputFile.close(); // Close the file

  // Initiating variables for page faults, FIFO queue, in-memory pages
  int pageFaults = 0;
  queue<int> fifoQueue;
  // unordered_map<int, bool> in_memory;
  int in_memory[num_blocks];
  // memset(in_memory, 0, sizeof(in_memory));
  fill_n(in_memory, num_blocks, 0);

  // Iterate through page references and implement the FIFO algorithm
  for (int i = 0; i < pageReferences.size(); i++)
  {
    int page = pageReferences[i];
    if (in_memory[page])
    { // check if page is already in memory
      // cout << "Page " << page << " already in memory." << endl;
    }
    else
    {
      pageFaults++;
      if (fifoQueue.size() == num_frames)
      {                                      // check if all frames are full
        int victim_page = fifoQueue.front(); // get the first page in the FIFO queue
        fifoQueue.pop();                     // remove the first page from the FIFO queue
        in_memory[victim_page] = 0;          // set the page to not in memory
        // in_memory[victim_page] = false;
        // cout << "Page " << victim_page << " evicted from memory." << endl;
      }
      fifoQueue.push(page);
      in_memory[page] = 1;
      // in_memory[victim_page] = true;
      // cout << "Page " << page << " brought into memory." << endl;
    }
  }
  cout << "** Fifo Page Replacement Algorithm **" << endl;
  cout << "Total number of page faults: " << pageFaults << endl;
  cout << "Total number of hits: " << (pageCount - pageFaults) << endl;

  return 0;
}
