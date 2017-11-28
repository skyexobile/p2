bool wayToSort( Tuple const& a, Tuple const& b) {
  return a.getField("homework").integer < b.getField("homework").integer;
}


MainMemory mem;
Disk disk;
SchemaManager schema_manager(&mem,&disk);

int numBlocks = tablePtrs[tName]->getNumOfBlocks();
//Read the blocks into memory 10 blocks at a time
  // MainMemory::getTuples(int memory_block_begin,int num_blocks)
      //gets tuples from a range of blocks
  vector<Tuple> sort_tuples = mem.getTuples(0, 10);
  //need to figure out what we're sorting by for wayToSort
  sort(sort_tuples.begin(), sort_tuples.end(), wayToSort);
  mem.setTuples(0,sort_tuples);
  cout << "Now the sorted memory contains: " << endl;
  cout << mem << endl;
  //Write the sublists back onto disk but make sure to store the starting index of the sublist
  //bool Relation::setBlocks(int relation_block_index, int memory_block_index, int num_blocks)
    if (numBlocks-i >= 10) {
        tablePtrs[tName]->setBlocks(i,0,10);
    } else {
        tablePtrs[tName]->setBlocks(i,0,numBlocks-i);
    }
  //Read the first block of each sublist into memory → getBlock()
  //Reading the i block of the relation into memory" << endl;
  for (int j = 0; j< tablePtrs[tName]->getNumOfBlocks(); j++){
  //tablePtrs[tName]->getBlock(i,j);
  }
  cout << *tablePtrs[tName] << endl;

  //Find the minimum among the smallest tuples in each sublist and output it → getTuples(), compare the smallest in each tuple vector using the compare function defined for sort.

}
