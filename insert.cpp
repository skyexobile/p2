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
  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  readQuote();
  readWord(temp);
  if(!temp[0]) {
      cerr << "value: value not read";
  }
  readQuote();
  inDataObj->field_values.push_back(string(temp));
  return;
}

void valueList(insertData *inDataObj){
  value(inDataObj);
  if(readComma()){
    valueList(inDataObj);
  }
  return;
}

void insertTuples(insertData *inDataObj){
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    readWord(temp);
    if(!temp[0]) {
        cerr << "insertTuples: nothing read";
    }

    if(strcmp(temp, "SELECT") == 0){
        //selectStmt(inDataObj);
        //return;
    } else if(strcmp(temp, "VALUES") == 0){
        if(readParen()){
            valueList(inDataObj);
        }
        if(readParen()){
            return;
        }
    }
    cerr << "insertTuples: closing parentheses not read\n";
}

void attrList(insertData *inDataObj) {
    char *attrNameBuf;
    attrNameBuf = (char *)malloc(20*sizeof(char));
    attrName(attrNameBuf);
    inDataObj->field_names.push_back(string(attrNameBuf));
    if(readComma()){
      attrList(inDataObj);
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
