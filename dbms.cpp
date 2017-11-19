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
#include "Block.h"
#include "Config.h"
#include "Disk.h"
#include "Field.h"
#include "MainMemory.h"
#include "Relation.h"
#include "Schema.h"
#include "SchemaManager.h"
#include "Tuple.h"
#include <unordered_map>
#include <iterator>

void appendTupleToRelation(Relation* relation_ptr, MainMemory& mem, int memory_block_index, Tuple& tuple) {
    Block* block_ptr;
    block_ptr=mem.getBlock(memory_block_index);
    if (relation_ptr->getNumOfBlocks()!=0) {
        cout << "The relation is not empty" << endl;
        cout << "Read the last block of the relation into memory block 0:" << endl;
        relation_ptr->getBlock(relation_ptr->getNumOfBlocks()-1,memory_block_index);
        if (!block_ptr->isFull()) {
            cout << "The block is not full" << endl;
            block_ptr->appendTuple(tuple); // append the tuple
            relation_ptr->setBlock(relation_ptr->getNumOfBlocks()-1,memory_block_index); //write back to the relation
            return;
        }
    }

    block_ptr->clear();
    block_ptr->appendTuple(tuple); // append the tuple
    cout << "Write to new block of the relation" << endl;
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
    char *stmtBuf, *stmtBuf2;
    stmtBuf= (char *)malloc(10*sizeof(char));
    readWord(stmtBuf);
    while(stmtBuf[0] != '0') {
        /* call the corresponding subroutine */

        if (strcmp(stmtBuf, "CREATE") == 0){
            readWord(stmtBuf);
            if (strcmp(stmtBuf, "TABLE") == 0){
                createTableData crTableObj;
                createTable(&crTableObj);
                cout << crTableObj << endl;
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
                cout << inDataObj << endl;
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
                cout << "Now the memory contains: " << endl;
                cout << mem << endl;
                cout << "Now the relation contains: " << endl;
                cout << *(tablePtrs[inDataObj.relationName]) << endl << endl;
                //cout << "current schema and relations" << endl << schema_manager << endl;

            }
        } else if (strcmp(stmtBuf, "SELECT") == 0) {
            selectData selDataObj;
            if(readStar())
            {
                selectStmt(&selDataObj);
                cout << selDataObj << endl;
                string tName = selDataObj.relation_names[0]; // assuming selecting from single table
                cout << "Table " << tName  << " contains: "<< endl;
                //memory block 0 contains:
                for(int i = 0; i< (tablePtrs[tName]->getNumOfBlocks());i++){
                    tablePtrs[tName]->getBlock(i,0);
                    Block* block_ptr = mem.getBlock(0);
                    vector<Tuple> tuples = block_ptr->getTuples();
                    copy(tuples.begin(),tuples.end(),ostream_iterator<Tuple>(cout,"\n"));
                }
                cout << endl;
            }
            else{
                selectStmt(&selDataObj);
                cout << selDataObj << endl;
                string tName = selDataObj.relation_names[0]; // assuming selecting from single table
                cout << "Attributes selected: " << endl;
                for (const auto& i:selDataObj.column_names) {
                    cout << i << '\t';
                }
                cout << endl;

                cout << "Table " << tName  << " contains: "<< endl;
                //memory block 0 contains:
                for (const auto& j:selDataObj.column_names) {

                    for(int i = 0; i< (tablePtrs[tName]->getNumOfBlocks());i++){
                        tablePtrs[tName]->getBlock(i,0);
                        Block* block_ptr = mem.getBlock(0);
                        vector<Tuple> tuples = block_ptr->getTuples();

                        for (const auto& i:tuples) {
                            Schema tuple_schema = i.getSchema();
                            string fieldName = j;
                            if (tuple_schema.getFieldType(j)==INT){
                                cout << i.getField(fieldName).integer << "\t";
                            }
                            else{
                                cout << *(i.getField(fieldName).str) << "\t";
                            }
                            cout << endl;
                        }
                    }
                    //copy(tuples.begin(),tuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));
                }//143 forloop end
            }
        }
        readWord(stmtBuf);
    }
}
