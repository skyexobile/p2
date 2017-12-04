#ifndef JOIN_H
#define JOIN_H
#include "common.h"
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
void join(Relation *comb_relPtr, Relation *join_relPtr, vector<string> relation_names, node *searchTree, MainMemory& mem, Disk& disk);
void concatenate(Tuple& combTuple, const Tuple& relTuple, int offset);
void collectTuple(Tuple& combTuple, Relation *join_relPtr, vector<Tuple>& selTuples);
#endif

