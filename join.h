#ifndef JOIN_H
#define JOIN_H
#include "common.h"
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;
void join(Relation *join_relPtr, vector<string> relation_names, unordered_map<string, Relation *>& tablePtrs, node *searchTree, MainMemory& mem, Disk& disk);
void concatenate(Tuple& joinTuple, const Tuple& relTuple, int offset);
#endif

