#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include "io.h"
#include "parseTree.h"
#include "select.h"
#include "common.h"
#include "stmtDataStructs.h"

void value(insertData *inDataObj){
  char *temp, *quoteBuf;
  temp = (char *)malloc(20*sizeof(char));
  readQuote();
  readWord(temp);
  readQuote();
  inDataObj->field_values.push_back(string(temp));
  return;
}
void valueList(insertData *inDataObj){

  value(inDataObj);
  if(readComma()){
    valueList(inDataObj);
  }
  else if(readParen()){
      return;
  }
  return;
}

void insertTuples(insertData *inDataObj){

  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  readWord(temp);
  if(strcmp(temp, "SELECT") == 0){
    //selectStmt(inDataObj);
    //return;
  }
  if(strcmp(temp, "VALUES") == 0){
    if(readParen()){
        valueList(inDataObj);
    }
    if(readParen()){
    }
  }
    return;
}

void attrList(insertData *inDataObj) {
    char *attrNameBuf;
    attrNameBuf = (char *)malloc(20*sizeof(char));
    attrName(attrNameBuf);
    inDataObj->field_names.push_back(string(attrNameBuf));
    if(readComma()){
      attrList(inDataObj);
    }
    if(readParen()){
    }
    return;
}

//use tablename function


void insertStmt(insertData *inDataObj) {
    char *tableNameBuf = (char*)malloc(10*sizeof(char));
    tableName(tableNameBuf);
    inDataObj->relationName = tableNameBuf;
    if(readParen()){
      //append and call attribute listf
      attrList(inDataObj);
      if(readParen()){
        insertTuples(inDataObj);
      }
    }
    return;
}
