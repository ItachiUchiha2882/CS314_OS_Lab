#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>
#include <utility>
#include <string>
#include <cmath>

using namespace std;

int MAX_BLOCK_SIZE = 0;
int MIN_BLOCK_SIZE = 0;
struct BuddyBlock *root = NULL;
map<int, char> processMap;

struct BuddyBlock
{
  int p_id;
  int size;           // size of the block
  int start;          // starting address of the block
  bool free;          // flag indicating whether the block is free or allocated
  int size_used;      // size of the block used
  BuddyBlock *left;   // pointer to left buddy block
  BuddyBlock *right;  // pointer to right buddy block
  BuddyBlock *parent; // pointer to parent block

  // constructor
  BuddyBlock(int p_idc, int sz, int st, bool fr, int s_u, BuddyBlock *l, BuddyBlock *r, BuddyBlock *p) : p_id(p_idc), size(sz), start(st), free(fr), size_used(s_u), left(l), right(r), parent(p) {}
};

struct Request
{
  int pid;        // process id
  int size;       // size of the memory block
  bool terminate; // flag indicating whether the process is terminating or allocating memory

  // constructor
  Request(int p, int s, bool t) : pid(p), size(s), terminate(t) {}
};

template <typename K, typename V>
void printMap(const map<K, V> &m)
{
  cout << "Printing map: \n";
  cout << "{";
  for (const auto &kv : m)
  {
    cout << kv.first << ": " << kv.second << ", ";
  }
  cout << "}" << endl;
}

bool compareBlocks(BuddyBlock *b1, BuddyBlock *b2)
{
  if (b1->size != b2->size)
  {
    return b1->size < b2->size; // smaller blocks first
  }
  else
  {
    return b1->start > b2->start; // most recently freed block first
  }
}

class BuddySystem
{
private:
  int lowerSize;                   // lower size bound for blocks
  int upperSize;                   // upper size bound for blocks
  BuddyBlock *root;                // root block of the binary tree
  map<int, BuddyBlock *> blockMap; // map of active blocks
  vector<BuddyBlock *> freeList;   // list of free blocks, sorted by size and most recently freed
  queue<Request> requestQueue;     // queue of pending requests

public:
  BuddySystem(int lower, int upper)
  {
    lowerSize = lower;
    upperSize = upper;
    root = new BuddyBlock(-1, upperSize, 0, true, 0, NULL, NULL, NULL);
    blockMap[0] = root;
    // printMap(blockMap);
    freeList.push_back(root);
  }

  void allocateMemory(int pid, int size)
  {
    Request request(pid, size, false);
    requestQueue.push(request);
    processRequests();
    // cout << "Allocated " << size << " bytes for process " << pid << endl;
  }

  void releaseMemory(int pid)
  {
    Request request(pid, 0, true);
    requestQueue.push(request);
    processRequests();
    // cout << "Released memory for process " << pid << endl;
  }

  void processRequests()
  {
    while (!requestQueue.empty())
    {
      Request request = requestQueue.front(); // Get the first element in the request queue
      requestQueue.pop();                     // Remove the first element from the request queue.
      if (request.terminate)
      {
        releaseProcess(request.pid);
      }
      else
      {
        allocateProcess(request.pid, request.size);
      }
    }
  }

  // This function allocates memory to a process with the given pid and size.
  void allocateProcess(int pid, int size)
  {
    // Finding the free block of memory with appropriate size if available or returning error message if not found.
    BuddyBlock *block = findFreeBlock(size);
    // cout << "** In allocateProcess - Allocating memory for process " << pid << endl;
    // cout << "block " << (block == NULL) << endl;
    if (block == NULL)
    {
      cerr << "Memory allocation error: not enough free memory" << endl;
      return;
    }
    block->p_id = pid;
    block->size_used = size;
    // Marking the block as allocated by the current process.
    block->free = false;

    // Associating the block with the given process id in the map.
    blockMap[pid] = block;

    // Marking all parent blocks as allocated as well.
    while (block->parent != NULL)
    {
      block = block->parent;
      block->free = false;
    }

    // Printing success message and printing the state of all blocks.
    // cout << "Allocated " << blockMap[pid]->size << " bytes for process " << pid << endl;
    // printState();
  }

  // This function releases memory which was previously allocated to the process with provided pid.
  void releaseProcess(int pid)
  {
    // Getting the block of memory associated with provided pid from the map.
    BuddyBlock *block = blockMap[pid];
    // cout << "** In releaseProcess - Releasing memory for process " << pid << endl;
    // cout << "block " << (block == NULL) << endl;

    // Returning error message if no such block is found in the map.
    if (block == NULL)
    {
      cerr << "Memory release error: invalid process id (no such block is found)" << endl;
      return;
    }

    // Marking the block as free and removing it from the map.
    block->free = true;
    blockMap.erase(pid);

    // Iterating over all parent blocks of the released block for possible merging until we reach the root node.
    while (block != NULL)
    {
      // Get the buddy block of the given block.
      BuddyBlock *buddy = getBuddy(block);

      // If the buddy exists and is free, merge adjacent free buddies.
      if (buddy != NULL && buddy->free)
      {
        // Remove buddy from the free list since it will be merged with block.
        freeList.erase(remove(freeList.begin(), freeList.end(), buddy), freeList.end());
        // Make sure that block is the leftmost block out of the two blocks and swap if necessary.
        if (buddy < block)
        {
          swap(block, buddy);
        }

        // cout << "AFTER: block->parent->size " << (block->parent->size) << endl;
        // Combine the sizes of both blocks into one.
        block->size *= 2;
        // cout << "BEFORE MERGE : block->right null : " << (block->right == NULL) << endl; 
        // Merge the right block of the buddy with block's right block.
        block->right = buddy->right;
        // cout << "AFTER MERGE : block->right null : " << (block->right == NULL) << endl; 
        // Set the parent of the right block to block if it exists.
        if (block->right != NULL)
        {
          block->right->parent = block;
        }
        // Mark the merged block as free.
        block->free = true;
        // buddy->free = true;
        // block = NULL;
        // block->left = NULL;
        // buddy = NULL;
        if(block == block->parent->left){
          block->parent->right = NULL;
          block->parent->left = NULL;
        } else if(block == block->parent->right){
          block->parent->left = NULL;
          block->parent->right = NULL;
        }
        // buddy->left = NULL;
      }
      else
      {
        // If buddy is not free, stop merging and add current block in the free list.
        block->free = true;
        freeList.insert(lower_bound(freeList.begin(), freeList.end(), block, compareBlocks), block);
        break;
      }
      // Moving up to the parent block.
      block = block->parent;
      // print_status(root);
      // cout<<"_______________\n";
    }
    // Printing success message and printing the state of all the blocks.
    // cout << "Released memory for process " << pid << endl;
    // printState();
  }

  // Finds the smallest free block that can accommodate the requested size.
  BuddyBlock *findFreeBlock(int size)
  {
    // Search the free list for a block of suitable size.
    for (vector<BuddyBlock *>::iterator it = freeList.begin(); it != freeList.end(); ++it)
    {
      BuddyBlock *block = *it;

      // If a block of suitable size is found, remove it from the free list.
      if (block->size >= size)
      {
        freeList.erase(it);

        // Split the block until its size matches the requested size.
        while ((block->size) / 2 > size && (block->size) / 2 >= MIN_BLOCK_SIZE)
        {
          // Create two new blocks by halving the size of the current block.
          BuddyBlock *leftBlock = new BuddyBlock(0, block->size / 2, block->start, true, 0, NULL, NULL, block);
          BuddyBlock *rightBlock = new BuddyBlock(0, block->size / 2, block->start + block->size / 2, true, 0, NULL, NULL, block);

          // Update the left and right pointers of the current block to point to its children.
          block->left = leftBlock;
          block->right = rightBlock;

          // Mark the current block as non-free and mark its children as free.
          block->free = false;
          leftBlock->parent = block;
          rightBlock->parent = block;

          // Add the right block to the free list, keeping it ordered by address.
          freeList.insert(lower_bound(freeList.begin(), freeList.end(), rightBlock, compareBlocks), rightBlock);

          // Select the left child block as the new block to be split or returned as a result.
          block = leftBlock;
        }

        // Mark the resulting block as non-free and return it.
        return block;
      }
    }

    // Return NULL if no block of suitable size is found.
    return NULL;
  }

  // Returns the buddy block of a given block, else returns NULL if no buddy exists
  BuddyBlock *getBuddy(BuddyBlock *block)
  {
    // If the parent of the given block is NULL, then it's the root and has no buddy, so return NULL.
    if (block->parent == NULL)
    {
      return NULL;
    }
    // If the given block is the left child of its parent, its buddy is the right child of the parent.
    else if (block == block->parent->left)
    {
      return block->parent->right;
    }
    // Otherwise, the given block is the right child of its parent, and therefore its buddy is the left child of the parent.
    else
    {
      return block->parent->left;
    }
  }

  // This function prints the current state of the memory
  void mainPrint()
  {
    print_status(root);
  }

  void print_status(struct BuddyBlock *node)
  { // only root should be passed
    if (node == NULL)
    {
      return;
    }
    print_status(node->left);
    print_status(node->right);
    if (node->left == NULL && node->right == NULL)
    {
      if (node->free == false)
      {
        cout << processMap[node->p_id] << ": " << node->size_used << endl;
      }
      else
      {
        cout << "Free Block: " << node->size << endl;
      }
    }
  }
};

void send_query(char pname_in, int pbytes)
{
  printf("Query: %c %d\n", pname_in, pbytes);
}

void clear(struct BuddyBlock *node)
{
  if (node == NULL)
  {
    return;
  }
  clear(node->left);
  clear(node->right);
  free(node);
}

void sfileRead()
{
  struct BuddyBlock *root = NULL;
  int T;
  FILE *fp;
  fp = fopen("output.txt", "w");
  fclose(fp);
  FILE *in;
  in = fopen("input.txt", "r");
  // in = fopen(argv[1], "r");
  // ifstream in ("input.txt");

  fscanf(in, "%d\n", &T);
  int i = 0;
  for (int i = 0; i < T; i++)
  {
    int U, L, pbytes;

    fscanf(in, "%d %d\n", &U, &L);
    clear(root);
    char pname_in;
    char line[20];
    while (fgets(line, 20, in) != NULL)
    {
      if (line[0] == '\n')
      {
        break;
      }
      sscanf(line, "%c %d\n", &pname_in, &pbytes);
      // send_query(pname_in, pbytes);
      printf("Query: %c %d\n", pname_in, pbytes);
      // ------
      string line;
      int processId, size;
      int upsize = 2 ^ U, lowsize = 2 ^ L;
      BuddySystem buddy(lowsize, upsize);
      buddy.allocateMemory(pname_in, pbytes); // allocate 64 bytes for process 1
      char *temp;
    }
    printf("end of this test, with U: %d and L: %d\n", U, L);
  }
  fclose(in);
}

int main(int argc, char *argv[])
{

  // cout << "** In main **" << endl;
  // cout << "----- Reading from file -------" << endl;
  int T;
  FILE *fp;
  fp = fopen("output.txt", "w");
  fclose(fp);

  if (argc > 2) // argc contains argument count. Argument count should be 2 for execution to proceed in this case
  {             // the arguments are stored using pointer array *argv[]
    printf("too many arguments. enter exactly one file name.");
    exit(1);
  }
  else if (argc < 2)
  {
    printf("too few arguments. enter exactly one file name.");
    exit(2);
  }

  FILE *in;
  // in = fopen("input.txt", "r");
  in = fopen(argv[1], "r");

  fscanf(in, "%d\n", &T);
  int i = 0;
  for (int i = 0; i < T; i++)
  {
    int U, L, pbytes;
    fscanf(in, "%d %d\n", &U, &L);
    clear(root);
    char pname_in;
    char line[20];
    int upsize = pow(2, U), lowsize = pow(2, L);
    MAX_BLOCK_SIZE = upsize;
    MIN_BLOCK_SIZE = lowsize;
    // cout << "upsize: " << upsize << " lowsize: " << lowsize << endl;
    BuddySystem buddy(lowsize, upsize);
    while (fgets(line, 20, in) != NULL)
    {
      if (line[0] == '\n')
      {
        break;
      }
      sscanf(line, "%c %d\n", &pname_in, &pbytes);
      // send_query(pname_in, pbytes);
      // printf("Query: %c %d\n", pname_in, pbytes);
      // --------------
      // int processId, size;
      int pid_int = (int)pname_in - 64;
      processMap[pid_int] = pname_in;
      if (pbytes == 0)
      {
        // cout << "-- Releasing memory for process " << pid_int << endl;
        // cout << "-- Releasing memory for process " << pname_in << endl;
        buddy.releaseMemory(pid_int);
      }
      else
      {
        // cout << "++ Allocating memory for process " << pid_int << endl;
        // cout << "++ Allocating memory for process " << pname_in << endl;
        buddy.allocateMemory(pid_int, pbytes); // allocate 64 bytes for process 1
      }
      // ------------------
    }
    buddy.mainPrint();
    cout<<"\n";
    // printf("end of this test, with U: %d and L: %d\n", U, L);
  }
  fclose(in);
  return 0;
}