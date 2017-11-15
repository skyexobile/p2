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

void appendTupleToRelation(Relation* relation_ptr, MainMemory& mem, int memory_block_index, Tuple& tuple) {
  Block* block_ptr;
  if (relation_ptr->getNumOfBlocks()==0) {
    cout << "The relation is empty" << endl;
    cout << "Get the handle to the memory block " << memory_block_index << " and clear it" << endl;
    block_ptr=mem.getBlock(memory_block_index);
    block_ptr->clear(); //clear the block
    block_ptr->appendTuple(tuple); // append the tuple
    cout << "Write to the first block of the relation" << endl;
    relation_ptr->setBlock(relation_ptr->getNumOfBlocks(),memory_block_index);
  } else {
    cout << "Read the last block of the relation into memory block 5:" << endl;
    relation_ptr->getBlock(relation_ptr->getNumOfBlocks()-1,memory_block_index);
    block_ptr=mem.getBlock(memory_block_index);

    if (block_ptr->isFull()) {
      cout << "(The block is full: Clear the memory block and append the tuple)" << endl;
      block_ptr->clear(); //clear the block
      block_ptr->appendTuple(tuple); // append the tuple
      cout << "Write to a new block at the end of the relation" << endl;
      relation_ptr->setBlock(relation_ptr->getNumOfBlocks(),memory_block_index); //write back to the relation
    } else {
      cout << "(The block is not full: Append it directly)" << endl;
      block_ptr->appendTuple(tuple); // append the tuple
      cout << "Write to the last block of the relation" << endl;
      relation_ptr->setBlock(relation_ptr->getNumOfBlocks()-1,memory_block_index); //write back to the relation
    }
  }
}

int main() {

    /* Initialize the main memory and disk */
    MainMemory mem;
    Disk disk;
    cout << "The memory contains " << mem.getMemorySize() << " blocks" << endl;
    cout << mem << endl << endl;
    SchemaManager schema_manager(&mem,&disk);

    disk.resetDiskIOs();
    disk.resetDiskTimer();
    unordered_map<string, Relation *> tablePtrs;

    /* read the first word to check the type of statement */
    char *stmtBuf, *stmtBuf2;
    stmtBuf= (char *)malloc(10*sizeof(char));
    stmtBuf2 = (char *)malloc(10*sizeof(char));
    read(stmtBuf, false);

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
    }
    cout << endl << "Enter INSERT STATEMENT: " <<endl
    read(stmtBuf2, false);
    if (strcmp(stmtBuf2, "INSERT") == 0){
        read(stmtBuf2, false);
        if (strcmp(stmtBuf2, "INTO") == 0) {
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
          cout << "Created a tuple " << tuple << " through the relation" << endl;
          cout << "The tuple is invalid? " << (tuple.isNull()?"TRUE":"FALSE") << endl;
          Schema tuple_schema = tuple.getSchema();
          cout << "The tuple has schema" << endl;
          cout << tuple_schema << endl;
          cout << "A block can allow at most " << tuple.getTuplesPerBlock() << " such tuples" << endl;
          cout << "Clear the memory block 1" << endl;
          Block* block_ptr=mem.getBlock(1); //access to memory block 1
          block_ptr->clear(); //clear the block

          // A block stores at most 2 tuples in this case
          // -----------first tuple-----------
          cout << "Set the tuple at offset 0 of the memory block 1" << endl;
          block_ptr->appendTuple(tuple); // You can also use appendTuple()
          cout << "Now the memory block 0 contains:" << endl;
          cout << *block_ptr << endl;

          cout << "The block is full? " << (block_ptr->isFull()==1?"true":"false") << endl;
          cout << "The block currently has " << block_ptr->getNumTuples() << " tuples" << endl;
          cout << "The tuple at offset 0 of the block is:" << endl;
          cout << block_ptr->getTuple(0) << endl << endl;


          vector<Tuple> tuples=block_ptr->getTuples();
          cout << "Again the tuples in the memory block 1 are:" << endl;
          copy(tuples.begin(),tuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));
          cout << endl;
          cout << "Now memory contains: " << endl;
          cout << mem << endl;
          appendTupleToRelation(tablePtrs[inDataObj.relationName], mem, 0, tuple);
          cout << "Now the memory contains: " << endl;
          cout << mem << endl;
          cout << "Now the relation contains: " << endl;
          cout << *(tablePtrs[inDataObj.relationName]) << endl << endl;

      }
    }
  }
