#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include "io.h"
#include "common.h"
#include "select.h"
#include "insert.h"
#include "createTable.h"
#include "parseTree.h"
#include "stmtDataStructs.h"
#include "searchCondition.h"
#include <iterator>
#include <sstream>
#include <algorithm>
#include "join.h"
#include "delete.h"
#include <ctime>
#include <stdlib.h>
#include "sorting.h"

// Hash table made global since it is required in select.cpp, sorting.cpp and join.cpp
unordered_map<string, Relation *> tablePtrs;
string sortBy;
void appendTupleToRelation(Relation* relation_ptr, MainMemory& mem, int memory_block_index, Tuple& tuple) {
    Block* block_ptr;
    block_ptr=mem.getBlock(memory_block_index);
    if (relation_ptr->getNumOfBlocks()!=0) {
        relation_ptr->getBlock(relation_ptr->getNumOfBlocks()-1,memory_block_index);
        if (!block_ptr->isFull()) {
            block_ptr->appendTuple(tuple); // append the tuple
            relation_ptr->setBlock(relation_ptr->getNumOfBlocks()-1,memory_block_index); //write back to the relation
            return;
        }
    }

    block_ptr->clear();
    block_ptr->appendTuple(tuple); // append the tuple
    relation_ptr->setBlock(relation_ptr->getNumOfBlocks(),memory_block_index); //write back to the relation
}

int main() {

    /* Initialize the main memory and disk */
    MainMemory mem;
    Disk disk;
    SchemaManager schema_manager(&mem,&disk);

    /* read the first word to check the type of statement */
    char *stmtBuf;
    stmtBuf= (char *)malloc(10*sizeof(char));
    readWord(stmtBuf);
    int stmtCnt = 0;
    while(stmtBuf[0] != '0') {
        disk.resetDiskIOs();
        disk.resetDiskTimer();
        cout << endl << ++stmtCnt << endl;
        /* call the corresponding subroutine */

        if (strcmp(stmtBuf, "CREATE") == 0){
            readWord(stmtBuf);
            if (strcmp(stmtBuf, "TABLE") == 0){
                createTableData crTableObj;
                createTable(&crTableObj);
                vector<enum FIELD_TYPE> field_types;

                Schema schema(crTableObj.field_names,crTableObj.field_types);
                Relation* relation_ptr=schema_manager.createRelation(crTableObj.relationName,schema);
                tablePtrs[crTableObj.relationName] = relation_ptr;
            }
        }
        else if(strcmp(stmtBuf, "DROP") == 0){
            readWord(stmtBuf);
            if (strcmp(stmtBuf, "TABLE") == 0){
                char *tableNameBuf = (char *)malloc(10*sizeof(char));
                tableName(tableNameBuf);
                string relationName = string(tableNameBuf);
                schema_manager.deleteRelation(relationName);
                cout << "Dropped: "<< relationName << endl;
            }
        }
        else if (strcmp(stmtBuf, "INSERT") == 0){
            readWord(stmtBuf);
            if (strcmp(stmtBuf, "INTO") == 0) {
                insertData inDataObj;
                insertStmt(&inDataObj);
                //create tuple
                Tuple tuple = tablePtrs[inDataObj.relationName]->createTuple();
                //check to see how many fields to add
                int size = inDataObj.field_names.size();
                for(int i = 0; i <size ; i++){
                    string fieldValue = inDataObj.field_values[i];
                    if(isdigit(fieldValue[0])){
                        int fvInteger = stoi(fieldValue);
                        tuple.setField(inDataObj.field_names[i],fvInteger);
                    }
                    else {
                        tuple.setField(inDataObj.field_names[i],inDataObj.field_values[i]);
                    }
                }

                appendTupleToRelation(tablePtrs[inDataObj.relationName], mem, 0, tuple);
                cout << "Now the relation contains: " << endl;
                cout << *(tablePtrs[inDataObj.relationName]) << endl << endl;
                //cout << "current schema and relations" << endl << schema_manager << endl;

            }
        } else if (strcmp(stmtBuf, "SELECT") == 0) {
            selectData selDataObj;
            node *searchTreeRoot = nullptr;
            vector<Tuple> selTuples;
            searchTreeRoot = selectStmt(&selDataObj);
            cout << selDataObj << endl;
            if (searchTreeRoot != nullptr) {
                printTree(searchTreeRoot, 0);
            } else {
                cout << "no where clause" << endl;
            }
            if (!selDataObj.orderByCol.empty()) {
                sortBy = selDataObj.orderByCol;
            }
            if (selDataObj.relation_names.size() > 1) {
                vector<string> newFldNames;
                vector<enum FIELD_TYPE> newFldTypes;
                string newTableName;
                bool nJoinFlg = false;
                vector<string> nJoinRelNames, nJoinAttrs;
                Relation *comb_relPtr = nullptr, *join_relPtr = nullptr;
                vector<Tuple> lsortedTuples;
                vector<Tuple> rsortedTuples;
                //check if we have to do a natural join
                if ((selDataObj.relation_names.size() == 2) && (searchTreeRoot != nullptr)) {
                    bool flgCont = true;
                    vector<node *> vectChildNodes;
                    vectChildNodes = searchTreeRoot->subTree;
                    while(flgCont) {
                        for(auto & nodeptr: vectChildNodes){
                            // Check all the children of the searchTreeRoot or the AND node to check if there is an equality between 2 attributes
                            if(nodeptr->nodeType == "="){
                                // nodeType is the name of the attribute in case of an attribute name
                                string left = nodeptr->subTree[0]->nodeType;
                                string right = nodeptr->subTree[1]->nodeType;
                                size_t lPos = left.find(".");
                                size_t rPos = right.find(".");
                                if (lPos != left.npos && rPos != right.npos) {
                                    nJoinFlg = true;
                                    nJoinRelNames.push_back(left.substr(0,lPos));
                                    nJoinRelNames.push_back(right.substr(0,rPos));
                                    nJoinAttrs.push_back(left.substr(lPos+1));
                                    nJoinAttrs.push_back(right.substr(rPos+1));
                                    break;
                                } 
                            }
                        }
                        for(auto & nodeptr: vectChildNodes){
                            if(!nJoinFlg && nodeptr->nodeType == "AND"){
                                vectChildNodes = nodeptr->subTree;
                                flgCont = true;
                                break;
                            }
                            else {
                                flgCont = false;
                            }
                        }
                    }
                }

                for (const string& tName:selDataObj.relation_names) {
                    Relation *  relation_ptr = schema_manager.getRelation(tName);
                    Schema schema =  relation_ptr->getSchema();
                    vector<string> fieldNames= schema.getFieldNames();
                    vector<enum FIELD_TYPE> fieldTypes = schema.getFieldTypes();
                    // Prepend the relation name for every attribute name
                    for (string& fldName:fieldNames) {
                        fldName = tName +"."+ fldName;
                    }

                    // Create the schema for the combined tuple
                    newFldNames.insert(newFldNames.end(), fieldNames.begin(), fieldNames.end());
                    newFldTypes.insert(newFldTypes.end(), fieldTypes.begin(), fieldTypes.end());
                    newTableName = newTableName + tName;
                }

                if (!selDataObj.column_names.empty()) {
                    vector<string> joinFldNames;
                    vector<enum FIELD_TYPE> joinFldTypes;
                    for (auto fldName:selDataObj.column_names) {
                        joinFldNames.push_back(fldName);
                        int index = find(newFldNames.begin(), newFldNames.end(), fldName) - newFldNames.begin();
                        joinFldTypes.push_back(newFldTypes[index]);
                    }
                    // Create the schema for the join tuple
                    Schema joinSchema(joinFldNames, joinFldTypes);
                    join_relPtr=schema_manager.createRelation("JoinRelation" ,joinSchema);
                }

                Schema newSchema(newFldNames, newFldTypes);

                comb_relPtr=schema_manager.createRelation(newTableName,newSchema);
                node *searchTree = (searchTreeRoot == nullptr) ? nullptr : searchTreeRoot->subTree[0];
                if (nJoinFlg) {
                    // Only possible when we have 2 relations and nat join conditions are satisfied
                    natJoin(comb_relPtr, join_relPtr, nJoinRelNames, nJoinAttrs, searchTree, mem, disk);
                } else {
                    join(comb_relPtr, join_relPtr, selDataObj.relation_names, searchTree, mem, disk);
                }
                schema_manager.deleteRelation(newTableName);
                if (join_relPtr != nullptr) {
                    schema_manager.deleteRelation("JoinRelation");
                }

            } else {
                // Single relation SELECT query
                if (selDataObj.column_names.empty()) {
                    // One relation query, no need to prepend the relation name
                    string tName = selDataObj.relation_names[0]; // assuming selecting from single table
                    Schema schema = tablePtrs[tName]->getSchema();
                    selDataObj.column_names = schema.getFieldNames();
                }

                string tName = selDataObj.relation_names[0]; // assuming selecting from single table
                cout << "Attributes selected: " << endl;
                for (const auto& i:selDataObj.column_names) {
                    cout << i << '\t';
                }
                cout << endl;
                Schema tuple_schema = tablePtrs[tName]->getSchema();
                //Creating a schema
                vector<string> field_names;
                vector<enum FIELD_TYPE> field_types;
                for (const auto& j:selDataObj.column_names) {
                    field_names.push_back(j);
                    if (tuple_schema.getFieldType(j)==INT){
                        field_types.push_back(INT);
                    }
                    else{
                        field_types.push_back(STR20);
                    }
                }
                Schema selSchema(field_names,field_types);
                //need to create random name for each select query. otherwise, will say relation already exists

                int num = rand();
                string newTableName = "selectRelation" + to_string(num);
                Relation* relation_ptr=schema_manager.createRelation(newTableName,selSchema);
                Tuple selTuple = relation_ptr->createTuple();

                for(int i = 0; i< (tablePtrs[tName]->getNumOfBlocks());i++){
                    tablePtrs[tName]->getBlock(i,0);
                    Block *block_ptr = mem.getBlock(0);
                    vector<Tuple> tuples = block_ptr->getTuples();

                    // Get the required attributes from the tuple and create selTuple
                    for (const auto& t:tuples) {

                        if (!t.isNull()){ //for holes in relation after deletion
                            for (const auto& j:selDataObj.column_names) {
                                string fieldName = j;
                                if (tuple_schema.getFieldType(j)==INT){
                                    int val = t.getField(fieldName).integer;
                                    selTuple.setField(fieldName,val);

                                }
                                else{
                                    string name = *t.getField(fieldName).str;
                                    selTuple.setField(fieldName, name);
                                }
                            }
                            // Evaluate selTuple based on the where clause and select it if it satisfies the condition
                            if (searchTreeRoot != nullptr) {
                                // where clause in the statement
                                if (evalBool(searchTreeRoot->subTree[0], t)) {
                                    selTuples.push_back(selTuple);
                                }
                            }
                            else{
                                //regular select from statement without WHERE
                                selTuples.push_back(selTuple);
                            }
                        }
                    }

                }

                if (!selDataObj.orderByCol.empty()) {
                    sort(selTuples.begin(), selTuples.end(), wayToSort);
                }
                copy(selTuples.begin(),selTuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));
                cout << endl;
            }
        }
        else if (strcmp(stmtBuf, "DELETE") == 0) {
            deleteData delDataObj;
            node *del_searchTreeRoot;
            del_searchTreeRoot = deleteStmt(&delDataObj);
            string tName = delDataObj.relation_name;
            if (del_searchTreeRoot != nullptr) {
                //printTree(del_searchTreeRoot, 0);
                //go through every tuple in relation that satisfies evalbool searchCondition
                // read tuple into memory, set it to nullTuple(), which invalidates all tuples in that block
                // write memory block back to disk back to ith block of the relation
                for(int i = 0; i< (tablePtrs[tName]->getNumOfBlocks());i++){
                    tablePtrs[tName]->getBlock(i,0);
                    Block *block_ptr = mem.getBlock(0);
                    int numTuples = block_ptr->getNumTuples();
                    for(int offset = 0; offset < numTuples; offset++){
                        Tuple t = block_ptr->getTuple(offset);
                        if(evalBool(del_searchTreeRoot->subTree[0], t)){
                            block_ptr->nullTuple(offset);
                            tablePtrs[tName]->setBlock(i,0);
                        }

                    }
                }
            }
            else{//delete from everything
                //delete the block from [starting_block_index] to the last block
                tablePtrs[tName]->deleteBlocks(0);
            }
        }
        cout << "#Disk I/Os = " << disk.getDiskIOs() << endl;
        readWord(stmtBuf);
    }
}
