#include "sorting.h"
#include "common.h"
#include "Tuple.h"
using namespace std;
static string sortBy;
bool wayToSort( Tuple const& a, Tuple const& b) {
  return a.getField(sortBy).integer < b.getField(sortBy).integer;
}

vector<Tuple> sortTuples(Relation *sort_relPtr,string sortByVal, MainMemory& mem, Disk& disk) {
vector<Tuple> sorted;
sortBy = sortByVal;
int numBlocks = sort_relPtr->getNumOfBlocks();

for (int i = 0; i < numBlocks; i+=10) {
    //Read the blocks into memory 10 blocks at a time
    int numBlksRead = (numBlocks-i >= 10) ? 10 : (numBlocks-i);
    sort_relPtr->getBlocks(i,0,numBlksRead);
    vector<Tuple> tuplesInMem= mem.getTuples(0, numBlksRead);
//Read the blocks into memory 10 blocks at a time
  // MainMemory::getTuples(int memory_block_begin,int num_blocks)
      //gets tuples from a range of blocks
  //need to figure out what we're sorting by for wayToSort
  sort(tuplesInMem.begin(), tuplesInMem.end(), wayToSort);
  sorted.insert(sorted.end(), tuplesInMem.begin(), tuplesInMem.end());
}

return sorted;
}
