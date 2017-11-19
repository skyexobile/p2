#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include "io.h"
#include "stmtDataStructs.h"
#include "common.h"
void selSublist(selectData *selDataObj);

void tablelist(selectData *selDataObj){
  char *tableNameBuf = (char*)malloc(10*sizeof(char));
  tableName(tableNameBuf);
  cout << "table is : "<<string(tableNameBuf) <<endl;
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
    selSublist(selDataObj);

    return;

}

void selectStmt(selectData *selDataObj) {
    char* c= (char *)malloc(10*sizeof(char));
    readWord(c);
    //for SELECT *, next word will be FROM
    if(strcmp(c, "FROM") ==0){
        tablelist(selDataObj);
        return;
    }
    else if(strcmp(c, "DISTINCT")==0){
      //something special for distinct
    }
    else{
      cin.putback(' '); // need this otherwise col name and FROM will be combined

      //just read in first column name...
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

    return;
}
