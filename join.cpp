#include "common.h"
#include "searchCondition.h"
#include <iterator>
#include "join.h"
#include "sorting.h"

void natJoin(Relation *comb_relPtr, Relation *join_relPtr, vector<string> nJoinRelNames, vector<string> nJoinAttrs, node *searchTree, MainMemory& mem, Disk& disk) {
    // Call the sort functions to read the disk and get the tuples in sorted order
    vector<Tuple> selTuples;
    vector<Tuple> rel1Sorted = sortTuples(tablePtrs[nJoinRelNames[0]], nJoinAttrs[0], mem, disk);
    vector<Tuple> rel2Sorted = sortTuples(tablePtrs[nJoinRelNames[1]], nJoinAttrs[1], mem, disk);

    auto it1 = rel1Sorted.begin();
    auto it2 = rel2Sorted.begin();
    if ((*it1).getSchema().getFieldType(nJoinAttrs[0])==INT) {
        while(it1 != rel1Sorted.end() && it2 != rel2Sorted.end()) {
            int valRel1, valRel2;
            valRel1 = (*it1).getField(nJoinAttrs[0]).integer;
            valRel2 = (*it2).getField(nJoinAttrs[1]).integer;
            if (valRel1 == valRel2) {
                Tuple t = comb_relPtr->createTuple();
                concatenate(t, *it1, 0);
                concatenate(t, *it2, (*it1).getNumOfFields());
                if(!searchTree || evalBool(searchTree, t)) {
                    collectTuple(t,join_relPtr, selTuples);
                }
                it1++;
                it2++;
            } else if (valRel1 < valRel2) {
                it1++;
            } else {
                it2++;
            }
        }
    } else {

        while(it1 != rel1Sorted.end() && it2 != rel2Sorted.end()) {
            string valRel1, valRel2;
            valRel1 = *(*it1).getField(nJoinAttrs[0]).str;
            valRel2 = *(*it2).getField(nJoinAttrs[1]).str;
            if (valRel1 == valRel2) {
                Tuple t = comb_relPtr->createTuple();
                concatenate(t, *it1, 0);
                concatenate(t, *it2, (*it1).getNumOfFields());
                if(!searchTree || evalBool(searchTree, t)) {
                    collectTuple(t,join_relPtr, selTuples);
                }
            } else if (valRel1 < valRel2) {
                it1++;
            } else {
                it2++;
            }
        }
    }
    copy(selTuples.begin(),selTuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));
    cout << endl;
}


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
    cout << "Attributes Selected" << endl;
    Schema schema = join_relPtr ? join_relPtr->getSchema() : comb_relPtr->getSchema();
    for (auto fldName:schema.getFieldNames()) {
        cout << fldName << '\t';
    }
    cout << endl;
    copy(selTuples.begin(),selTuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));
    cout << endl;

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
        selTuples.push_back(combTuple);
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
       
