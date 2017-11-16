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
    cout << "The memory contains " << mem.getMemorySize() << " blocks" << endl;
    SchemaManager schema_manager(&mem,&disk);

    disk.resetDiskIOs();
    disk.resetDiskTimer();
    unordered_map<string, Relation *> tablePtrs;

    /* read the first word to check the type of statement */
    char *stmtBuf, *stmtBuf2;
    stmtBuf= (char *)malloc(10*sizeof(char));
    read(stmtBuf, false);
    cout << stmtBuf << endl;
    while(*stmtBuf != EOF) {
        cout << stmtBuf << endl;
        /* call the corresponding subroutine */
        if (strcmp(stmtBuf, "SELECT") == 0) {
            // selectStmt(root);
        }
        else if (strcmp(stmtBuf, "CREATE") == 0){
            read(stmtBuf, false);
            if (strcmp(stmtBuf, "TABLE") == 0){
                createTableData crTableObj;
                createTable(&crTableObj);
                cout << crTableObj << endl;
                vector<enum FIELD_TYPE> field_types;

                Schema schema(crTableObj.field_names,crTableObj.field_types);
                Relation* relation_ptr=schema_manager.createRelation(crTableObj.relationName,schema);
                tablePtrs[crTableObj.relationName] = relation_ptr;
                cout << "The schema has field types: " << endl;
                field_types=schema.getFieldTypes();
                for (int i=0;i<schema.getNumOfFields();i++) {
                    cout << (field_types[i]==0?"INT":"STR20") << "\t";
                }
            }
        } else if (strcmp(stmtBuf, "INSERT") == 0){
            read(stmtBuf, false);
            if (strcmp(stmtBuf, "INTO") == 0) {
                insertData inDataObj;
                insertStmt(&inDataObj);
                cout << inDataObj << endl;
                //create tuple
                Tuple tuple = tablePtrs[inDataObj.relationName]->createTuple();
                //check to see how many fields to add
                int size = inDataObj.field_names.size();
                for(int i = 0; i <size ; i++){
                    string fv= inDataObj.field_values[i];
                    if(isdigit(fv[0])){
                        int fvInteger = stoi(fv);
                        tuple.setField(inDataObj.field_names[i],fvInteger);

                    }
                    else{
                        tuple.setField(inDataObj.field_names[i],inDataObj.field_values[i]);
                    }
                }

                appendTupleToRelation(tablePtrs[inDataObj.relationName], mem, 0, tuple);
                cout << "Now the memory contains: " << endl;
                cout << mem << endl;
                cout << "Now the relation contains: " << endl;
                cout << *(tablePtrs[inDataObj.relationName]) << endl << endl;

            }
        }
        read(stmtBuf, false);
    }
}
