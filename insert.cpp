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
  quoteBuf = (char *)malloc(2*sizeof(char));
  read(quoteBuf,true);
  read(temp, false);
  read(quoteBuf, true);
  inDataObj->field_values.push_back(string(temp));
  return;

}
void valueList(insertData *inDataObj){

  value(inDataObj);
  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  read(temp, true);
  if(strcmp(temp, ",") ==0){
    valueList(inDataObj);
  }
  else if(strcmp(temp,")") == 0){
    putChar(')');
  }
  return;
}

void insertTuples(insertData *inDataObj){

  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  read(temp, false);
  if(strcmp(temp, "SELECT") == 0){
    //selectStmt(inDataObj);
    return;
  }
  else if(strcmp(temp, "VALUES") == 0){

    read(temp, true);

    if(strcmp(temp, "(") == 0){
            valueList(inDataObj);
      read(temp, true);
    }
    if(strcmp(temp, ")" )== 0){

    }
  }
    return;


}

void attrList(insertData *inDataObj) {
    node *attrListNode, *commaNode;
    char *temp;
    temp = (char *)malloc(3*sizeof(char));
    char *attrNameBuf;
    attrNameBuf = (char *)malloc(20*sizeof(char));
    attrName(attrNameBuf);
    inDataObj->field_names.push_back(string(attrNameBuf));
    read(temp, true);
    if(strcmp(temp, ",") == 0){

      attrList(inDataObj);
    }
    if(strcmp(temp, ")") ==0){
      putChar(')');
    }
    return;
}

//use tablename function


void insertStmt(insertData *inDataObj) {
    char *c;
    c = (char *)malloc(20*sizeof(char));
    char *tableNameBuf = (char*)malloc(10*sizeof(char));
    tableName(tableNameBuf);
    inDataObj->relationName = tableNameBuf;
    //inDataObj->field_names.push_back(string(tableNameBuf));
    read(c, true);
    if(c[0] == '\0'){
      read(c,true);
    }
    if(strcmp(c, "(") == 0){
      //append and call attribute listf
      attrList(inDataObj);
      read(c, true);
      if(strcmp(c, ")") == 0){
        insertTuples(inDataObj);

      }

    }

    return;
}
