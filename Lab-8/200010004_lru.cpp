#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
  if (argc != 5)
  {
    cout << "Please enter the correct number of arguments." << endl;
    cout << "Usage: ./200010004_lru <num_pages> <num_frames> <num_blocks> <filename>" << endl;
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

  vector<int> pageReferences;
  int page, pageCount = 0;
  ifstream inputFile(filename);
  if (!inputFile.is_open())
  {
    cout << "Error: could not open the file " << filename << endl;
    return 1;
  }
  while (inputFile >> page)
  {
    pageCount++;
    pageReferences.push_back(page);
  }
  // cout << "Page count: " << pageCount << endl;
  inputFile.close(); // Close the file

  int pageFaults = 0;
  unordered_map<int, int> in_memory;
  vector<int> frameQueue;

  for (int i = 0; i < pageReferences.size(); i++)
  {
    int page = pageReferences[i];
    // Check if the page is already in memory
    if (in_memory.find(page) != in_memory.end())
    {
      // If the page is already in memory, move it to the back of the queue
      // cout << "Page " << page << " already in memory." << endl;
      for (int j = 0; j < frameQueue.size(); j++)
      {
        if (frameQueue[j] == page)
        {
          frameQueue.erase(frameQueue.begin() + j);
          break;
        }
      }
      frameQueue.push_back(page);
    }
    else
    {
      // If the page is not in memory, we need to replace a page
      if (in_memory.size() < num_frames)
      {
        // If there are empty num_frames, just use one of them
        in_memory[page] = in_memory.size();
        frameQueue.push_back(page);
        // cout << "Page " << page << " brought into memory." << endl;
      }
      else
      {
        // Otherwise, find the least recently used page and replace it
        int lruPage = frameQueue.front();
        in_memory.erase(lruPage);
        in_memory[page] = in_memory.size();
        frameQueue.erase(frameQueue.begin());
        frameQueue.push_back(page);
        // cout << "Page " << lruPage << " evicted from memory." << endl;
      }
      pageFaults++;
    }
  }

  cout << "** LRU Page Replacement Algorithm **" << endl;
  cout << "Total number of page faults: " << pageFaults << endl;
  cout << "Total number of hits: " << (pageCount - pageFaults) << endl;

  return 0;
}
