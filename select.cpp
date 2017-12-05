#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include <string>
#include "io.h"
#include "stmtDataStructs.h"
#include "common.h"
#include "searchCondition.h"

void selSublist(selectData *selDataObj);

void tablelist(selectData *selDataObj){
  char *tableNameBuf = (char*)malloc(10*sizeof(char));
  tableName(tableNameBuf);
  selDataObj->relation_names.push_back(string(tableNameBuf));
  //check if there is a comma ahead
  if(readComma()){
    tablelist(selDataObj);
  }
  return;
}

void selSublist(selectData *selDataObj) {
  char *colNameBuf;
  colNameBuf = (char *)malloc(20*sizeof(char));
  columnName(colNameBuf);
  if (!colNameBuf[0]) {
      cerr << "colName: colName not read";
  }
  selDataObj->column_names.push_back(string(colNameBuf));
  if(readComma()){
      selSublist(selDataObj);
  }
  return;
}

void selList(selectData *selDataObj) {
  //check for star, otherwise select-sublist
  if (readStar()) {
      return;
  }
    selSublist(selDataObj);
    return;
}

node* selectStmt(selectData *selDataObj) {
    char* c= (char *)malloc(20*sizeof(char));
    node* root=nullptr;
    readWord(c);
    if(strcmp(c, "DISTINCT")==0){
        //something special for distinct
    }
    else{
        //just read in first column name... so put it back into the stream
        for(int i = strlen(c)-1; i >=0; i--){
            cin.putback(c[i]);
        }
    }
    selList(selDataObj);
    //check for "FROM"
    readWord(c);
    if(strcmp(c, "FROM") == 0){
        tablelist(selDataObj);
    }
    readWord(c);
    if(strcmp(c, "WHERE") == 0) {
        string searchStrBuf;
        char p;
        cin.get(p);
        while (p != 'E' && p != '\n') {
            searchStrBuf.push_back(p);
            cin.get(p);
        }
        cin.putback(p);
        if (p == 'E') {
            cin.putback('D');
            cin.putback('R');
            cin.putback('O');
        }
        root = createTree(searchStrBuf);
    } else if(strcmp(c, "ORDER") == 0) {
        readWord(c);
        if(strcmp(c, "BY") == 0) {
            readWord(c);
            selDataObj->orderByCol = c;
            if (readPeriod()) {
                readWord(c);
                selDataObj->orderByCol = selDataObj->orderByCol + "." + c;
            }
        }
        return root;
    } else {
        for(int i = strlen(c)-1; i >=0; i--){
            cin.putback(c[i]);
        }
        return root;
    }

    readWord(c);
    if(strcmp(c, "ORDER") == 0) {
        readWord(c);
        if(strcmp(c, "BY") == 0) {
            readWord(c);
            selDataObj->orderByCol = c;
            if (readPeriod()) {
                readWord(c);
                selDataObj->orderByCol = selDataObj->orderByCol + "." + c;
            }
        }
        return root;
    }
    for(int i = strlen(c)-1; i >=0; i--){
        cin.putback(c[i]);
    }
    return root;
}
