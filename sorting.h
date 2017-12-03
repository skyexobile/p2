
#ifndef SORTING_H
#define SORTING_H
#include "common.h"
#include<algorithm>
bool wayToSort( Tuple const& a, Tuple const& b);
vector<Tuple> sortTuples(Relation *sort_relPtr,string sortByVal, MainMemory& mem, Disk& disk);

#endif
