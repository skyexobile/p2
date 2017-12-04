#include "common.h"
#include <string>
#include <cstring>
#include<iostream>
#include<iomanip>
using namespace std;
#include<vector>
#include "parseTree.h"
#include "io.h"
#include "stmtDataStructs.h"
#define WIDTH 15
void tableName(char *tableNameBuf);

bool wayToSort( Tuple const& a, Tuple const& b) {
  return a.getField(sortBy).integer < b.getField(sortBy).integer;
}

void columnName(char* colNameBuf){
    char *temp = (char *)malloc(20*sizeof(char)); 
    readWord(colNameBuf);
    if(readPeriod()){
      strcat(colNameBuf, ".");
      readWord(temp);
      strcat(colNameBuf, temp);
    }
    return;
}
void attrName(char *attrNameBuf){
    readWord(attrNameBuf);
    if (!attrNameBuf[0]) {
        cerr << "attrName: attrName not read";
    }
    return;
}

void tableName(char *tableNameBuf){
  readWord(tableNameBuf);
    if (!tableNameBuf[0]) {
        cerr << "tableName: tableName not read";
    }
  return;
}

void printTree(node* curr, int indent) {
    cout << setw(WIDTH*indent) << curr->nodeType << endl;
    vector<node *> vectChildNodes;
    vectChildNodes = curr->subTree;
    if (!vectChildNodes.empty()) {
        for (auto& nodeptr:vectChildNodes) {
            printTree(nodeptr, indent+1);
        }
        return;
    }
}
