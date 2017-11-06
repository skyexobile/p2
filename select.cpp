#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"
#include "common.h"
void selSublist(node *);

void tablelist(node *parent){
  //create non-terminal table-list node
  node *tablelistNode, *commaNode;
  tablelistNode = new node("tablelist", false);
  parent->subTree.push_back(tablelistNode);
  tableName(tablelistNode);
  //check if there is a comma ahead
  char *comBuf = (char *)malloc(3*sizeof(char));
  read(comBuf, true);
  if(comBuf[0] ==  '\0'){
    return;
  }
  else if (comBuf != nullptr) {
      commaNode = new node(",", true);
      tablelistNode-> subTree.push_back(commaNode);
      tablelist(tablelistNode);
  }
  return;

}

void columnName(node *parent){
    node *colNameNode, *commaNode;
    bool flgComma = false;
    colNameNode = new node("colName", false);
    parent->subTree.push_back(colNameNode);
    char *temp, *attrName;
    attrName = (char *)malloc(40*sizeof(char));
    temp = (char *)malloc(3*sizeof(char));
    read(temp, false);
    strcpy(attrName, temp);
    read(temp, true);
    if(temp != nullptr){
      //we have read either a . or ,
      if(strcmp(temp, "." ) == 0){
        strcat(attrName, temp);
        read(temp, false);
        strcat(attrName, temp);
      }
      else if(strcmp(temp, ",") ==0){
        flgComma = true;
      }
    }

    node *atrNameNode;
    atrNameNode = new node(attrName, true);
    colNameNode -> subTree.push_back(atrNameNode);
    if(flgComma){
        commaNode = new node(",", true);
        parent -> subTree.push_back(commaNode);
        selSublist(parent);
    }

    return;
}

void selSublist(node *parent) {
  cout << "selSublist" << endl;

    node *selSublistNode, *commaNode;
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    selSublistNode = new node("selSublist", false);
    parent -> subTree.push_back(selSublistNode);
    columnName(selSublistNode);
    read(temp, true);
    if(strcmp(temp, ",") ==0){
      commaNode = new node(",", true);
      parent->subTree.push_back(commaNode);
      selSublist(selSublistNode);
    }
    else{
      read(temp, false);
      if(strcmp(temp,"FROM") == 0){

        for(int i =(strlen(temp)-1); i>=0; i--){
          putChar(temp[i]);
          return;
        }
      }
    }
    return;
}

void selList(node *parent) {
  cout << "selList " << endl;
    node *selListNode, *starNode;
    selListNode = new node("selList", false);
    parent -> subTree.push_back(selListNode);
    char* c= (char *)malloc(3*sizeof(char));
    read(c, true);
    if(strcmp(c, "*") == 0){
      starNode = new node("*", true);
      parent->subTree.push_back(starNode);
    }
    else{

    selSublist(selListNode);

    return;
  }
}

void selectStmt(node *parent) {
    cout << "in selectStmt" << endl;
    node *selNode, *dNode, *fromNode;
    selNode = new node("SELECT", true);
    parent -> subTree.push_back(selNode);
    //check distinct
    char* c= (char *)malloc(10*sizeof(char));
    read(c, true);
    if(strcmp(c,"*")==0){
      selList(parent);
    }
    else{
      read(c, false);

    if(strcmp(c, "DISTINCT") ==0){
      dNode = new node("DISTINCT", true);
      parent->subTree.push_back(dNode);
}
    else{

      for(int i =(strlen(c)-1); i>=0; i--){
        putChar(c[i]);
      }
    }
    selList(parent);

}

    //check for "FROM"
    read(c, false);
    if(strcmp(c, "FROM") == 0){
      fromNode = new node("FROM", true);
      parent->subTree.push_back(fromNode);
      tablelist(parent);
    }
    else{
    }
    return;
}
