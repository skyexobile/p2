#include "common.h"
#include "searchCondition.h"
#include "join.h"

void join(Relation *join_relPtr, vector<string> relation_names, unordered_map<string, Relation *>& tablePtrs, node *searchTree, MainMemory& mem, Disk& disk) {
    int numRelns = relation_names.size();
    vector<vector<Tuple> > relTuples(numRelns);
    for (int rel_index = 0; rel_index < numRelns; rel_index++) {
        string tName = relation_names[rel_index];
        Relation *relation_ptr = tablePtrs[tName];
        int numBlocks = relation_ptr->getNumOfBlocks();

        for (int i = 0; i < numBlocks; i+=10) {
            //Read the blocks into memory 10 blocks at a time
            int numBlksRead = (numBlocks-i >= 10) ? 10 : (numBlocks-i); 
            tablePtrs[tName]->getBlocks(i,0,numBlksRead);
            vector<Tuple> tuplesInMem = mem.getTuples(0, numBlksRead);
            relTuples[rel_index].insert(relTuples[rel_index].end(), 
                    tuplesInMem.begin(), tuplesInMem.end());
        }
    }

    for (auto& t0:relTuples[0]) {
        // Add all the fields from t0 into t
        Tuple t = join_relPtr->createTuple();
        concatenate(t, t0, 0); 
        // Combine each tuple in the second relation with t0
        for (auto& t1:relTuples[1]) {
            // Add all the fields from t1 into t
            concatenate(t, t1, t0.getNumOfFields()); 
            if (numRelns > 2) {
                for (auto& t2:relTuples[2]) {
                    // Add all the fields from t2 into t
                    concatenate(t,t2,t0.getNumOfFields() + t1.getNumOfFields()); 
                    if(evalBool(searchTree, t)) {
                        t.printTuple();
                    }
                }
            } else {
                // Join of 2 relations
                    if(evalBool(searchTree, t)) {
                        t.printTuple();
                    }
            }
        }
    }
}

void concatenate(Tuple& joinTuple, const Tuple& relTuple, int offset) {
    for (int i=0; i < relTuple.getNumOfFields(); i++) {
        if (relTuple.getSchema().getFieldType(i)==INT) {
            joinTuple.setField(offset++, relTuple.getField(i).integer);
        } else if (relTuple.getSchema().getFieldType(i)==STR20) {
            joinTuple.setField(offset++, *relTuple.getField(i).str);
        }
    }
}
