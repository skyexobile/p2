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
#include <unordered_map>
#include <iterator>
#include<sstream>
#include<algorithm>
#include "join.h"


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

    disk.resetDiskIOs();
    disk.resetDiskTimer();
    unordered_map<string, Relation *> tablePtrs;

    /* read the first word to check the type of statement */
    char *stmtBuf;
    stmtBuf= (char *)malloc(10*sizeof(char));
    readWord(stmtBuf);
    while(stmtBuf[0] != '0') {
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
                cout << "Dropped: "<< relationName << endl << "Current schema and relations:" << endl << schema_manager << endl;
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
            node *searchTreeRoot;
            vector<Tuple> selTuples;

            if(readStar())
            {
                searchTreeRoot = selectStmt(&selDataObj);
                if (searchTreeRoot != nullptr) {
                    printTree(searchTreeRoot, 0);
                }
                // One ration query, no need to prepend the relation name
                string tName = selDataObj.relation_names[0]; // assuming selecting from single table
                Schema schema = tablePtrs[tName]->getSchema();
                selDataObj.column_names = schema.getFieldNames();                              
            } else {
                searchTreeRoot = selectStmt(&selDataObj);
                if (searchTreeRoot != nullptr) {
                    printTree(searchTreeRoot, 0);
                }
            }

            if (selDataObj.relation_names.size() > 1) {
                vector<string> newFldNames;
                vector<enum FIELD_TYPE> newFldTypes;
                string newTableName;
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

                Schema joinSchema(newFldNames, newFldTypes);
                Relation* relation_ptr=schema_manager.createRelation(newTableName,joinSchema);
                join(relation_ptr, selDataObj.relation_names, tablePtrs, searchTreeRoot->subTree[0], mem, disk);

            } else {

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
                Relation* relation_ptr=schema_manager.createRelation("selRelation",selSchema);
                Tuple selTuple = relation_ptr->createTuple();

                for(int i = 0; i< (tablePtrs[tName]->getNumOfBlocks());i++){
                    tablePtrs[tName]->getBlock(i,0);
                    Block *block_ptr = mem.getBlock(0);
                    vector<Tuple> tuples = block_ptr->getTuples();

                    // Get the required attributes from the tuple and create selTuple
                    for (const auto& t:tuples) {
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

                copy(selTuples.begin(),selTuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));
            }
        }
        readWord(stmtBuf);
    }
}
