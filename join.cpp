#include "common.h"
#include "searchCondition.h"
#include <iterator>
#include "join.h"

void join(Relation *comb_relPtr, Relation *join_relPtr, vector<string> relation_names, node *searchTree, MainMemory& mem, Disk& disk) {
    int numRelns = relation_names.size();
    vector<vector<Tuple> > relTuples(numRelns);
    vector<Tuple> selTuples;
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
        Tuple t = comb_relPtr->createTuple();
        concatenate(t, t0, 0);
        // Combine each tuple in the second relation with t0
        for (auto& t1:relTuples[1]) {
            // Add all the fields from t1 into t
            concatenate(t, t1, t0.getNumOfFields());

            if (numRelns > 2) {
                for (auto& t2:relTuples[2]) {
                    // Add all the fields from t2 into t
                    concatenate(t,t2,t0.getNumOfFields() + t1.getNumOfFields());
                    if(!searchTree || evalBool(searchTree, t)) {
                        collectTuple(t,join_relPtr, selTuples);
                    }
                }
            } else {
                // Join of 2 relations
                if(!searchTree || evalBool(searchTree, t)) {
                    collectTuple(t,join_relPtr, selTuples);
                }
            }
        }
    }
    sort(selTuples.begin(), selTuples.end(), wayToSort);
    copy(selTuples.begin(),selTuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));

}

void concatenate(Tuple& combTuple, const Tuple& relTuple, int offset) {
    for (int i=0; i < relTuple.getNumOfFields(); i++) {
        if (relTuple.getSchema().getFieldType(i)==INT) {
            combTuple.setField(offset++, relTuple.getField(i).integer);
        } else if (relTuple.getSchema().getFieldType(i)==STR20) {
            combTuple.setField(offset++, *relTuple.getField(i).str);
        }
    }
}

void collectTuple(Tuple& combTuple, Relation *join_relPtr, vector<Tuple>& selTuples) {
    if (join_relPtr == nullptr) {
        combTuple.printTuple();
    } else {
        Tuple joinTuple = join_relPtr->createTuple();
    for (auto fldName: joinTuple.getSchema().getFieldNames()) {
        if (joinTuple.getSchema().getFieldType(fldName)==INT) {
            joinTuple.setField(fldName, combTuple.getField(fldName).integer);
        } else if (joinTuple.getSchema().getFieldType(fldName)==STR20) {
            joinTuple.setField(fldName, *combTuple.getField(fldName).str);
        }
    }
    selTuples.push_back(joinTuple);
    }
}
       
