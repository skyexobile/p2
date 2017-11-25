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
#include<sstream>
#include<algorithm>

bool wayToSort( Tuple const& a, Tuple const& b) {
  return a.getField("homework").integer < b.getField("homework").integer;
}

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
                string tName = selDataObj.relation_names[0]; // assuming selecting from single table
                cout << "Attributes selected: " << endl;
                for (const auto& i:selDataObj.column_names) {
                    cout << i << '\t';
                }
                cout << endl;

                vector<Tuple> selTuples;
                //memory block 0 contains:
                Block* block_ptr = mem.getBlock(0);
                vector<Tuple> tuples = block_ptr->getTuples();
                Schema tuple_schema = tuples[0].getSchema();
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

                //Read the blocks into memory 10 blocks at a time
                for(int i = 0; i< (tablePtrs[tName]->getNumOfBlocks());i+=10){
                  //bool Relation::getBlocks(int relation_block_index, int memory_block_index, int num_blocks)
                  tablePtrs[tName]->getBlocks(i,0,tablePtrs[tName]->getNumOfBlocks());
                  cout << "Now the unsorted memory contains: " << endl;
                  cout << mem << endl;
                  // MainMemory::getTuples(int memory_block_begin,int num_blocks)
                      //gets tuples from a range of blocks
                  //Issue: blocks are not filling up when tuples are inserted
                    //i.e., each block contains one tuple instead of 2
                  //Sort the tuples in the 10 blocks
                  vector<Tuple> sort_tuples = mem.getTuples(0, block_ptr->getNumTuples()*tablePtrs[tName]->getNumOfBlocks());
                  //need to figure out what we're sorting by for wayToSort
                  sort(sort_tuples.begin(), sort_tuples.end(), wayToSort);
                  mem.setTuples(0,sort_tuples);
                  cout << "Now the sorted memory contains: " << endl;
                  cout << mem << endl;
                  //Write the sublists back onto disk but make sure to store the starting index of the sublist
                  //bool Relation::setBlocks(int relation_block_index, int memory_block_index, int num_blocks)
                  tablePtrs[tName]->setBlocks(i, 0, 10);
                  //Read the first block of each sublist into memory → getBlock()
                  //Reading the i block of the relation into memory" << endl;
                  for (int j = 0; j< tablePtrs[tName]->getNumOfBlocks(); j++){
                  tablePtrs[tName]->getBlock(i,j);
                  }
                  cout << *tablePtrs[tName] << endl;

                  //Find the minimum among the smallest tuples in each sublist and output it → getTuples(), compare the smallest in each tuple vector using the compare function defined for sort.

                }

                for(int i = 0; i< (tablePtrs[tName]->getNumOfBlocks());i++){
                    tablePtrs[tName]->getBlock(i,0);
                    block_ptr = mem.getBlock(0);
                    tuples = block_ptr->getTuples();


                    // Evaluate the tuples based on the where clause

                     for (const auto& j:selDataObj.column_names) {
                          string fieldName = j;
                          for (const auto& i:tuples) {
                          if (tuple_schema.getFieldType(j)==INT){
                              int val = i.getField(fieldName).integer;
                              selTuple.setField(fieldName,val);
                          }
                          else{
                              string name = *i.getField(fieldName).str;
                              selTuple.setField(fieldName, name);
                          }
                      }

                  }
                  selTuples.push_back(selTuple);
                }//143 forloop end
                copy(selTuples.begin(),selTuples.end(),ostream_iterator<Tuple,char>(cout,"\n"));
            }

        }
        //sort
        readWord(stmtBuf);
    }
}
