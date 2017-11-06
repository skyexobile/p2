#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"
void selSublist(node *);
void tableName(node *parent){
  node *tableNameNode, *tblNode;
  tableNameNode = new node();
  parent->subTree.push_back(tableNameNode);
  char *word = (char*)malloc(10*sizeof(char));
  read(word, false);
  tblNode = new node(word);
  tableNameNode->subTree.push_back(tblNode);
  return;
}
void tablelist(node *parent){
  //create non-terminal table-list node
  node *tablelistNode, *commaNode;
  tablelistNode = new node();
  parent->subTree.push_back(tablelistNode);
  tableName(tablelistNode);
  //check if there is a comma ahead
  char *comBuf = (char *)malloc(3*sizeof(char));
  read(comBuf, true);
  if(comBuf[0] ==  '\0'){
    return;
  }
  else if (comBuf != nullptr) {
      commaNode = new node(",");
      tablelistNode-> subTree.push_back(commaNode);
      tablelist(tablelistNode);
  }
  return;

}

void columnName(node *parent){
    node *colNameNode, *commaNode;
    colNameNode = new node();
    parent->subTree.push_back(colNameNode);
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    read(temp, false);
    node *atrNameNode;
    atrNameNode = new node(temp);
    colNameNode -> subTree.push_back(atrNameNode);
    char *comBuf = (char *)malloc(3*sizeof(char));
    read(comBuf, true);
    if(comBuf[0] ==  '\0'){
      return;
    }
    else if (comBuf != nullptr) {
        commaNode = new node(",");
        parent -> subTree.push_back(commaNode);
    }
    read(temp, false);
    if(strcmp(temp,"FROM") == 0){

      for(int i =(strlen(temp)-1); i>=0; i--){
        putChar(temp[i]);
        return;
      }
    }
    else{
      putChar(' ');
      for( int i =(strlen(temp)-1); i>=0; i--){
        putChar(temp[i]);
      }

      selSublist(parent);
    }
    return;
}

void selSublist(node *parent) {
  cout << "selSublist" << endl;
    node *selSublistNode, *commaNode;
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    selSublistNode = new node();
    parent -> subTree.push_back(selSublistNode);
    columnName(selSublistNode);

    return;
}

void selList(node *parent) {
  cout << "selList " << endl;
    node *selListNode, *starNode;
    selListNode = new node();
    parent -> subTree.push_back(selListNode);
    char* c= (char *)malloc(3*sizeof(char));
    read(c, true);
    if(strcmp(c, "*") == 0){
      starNode = new node("*");
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
    selNode = new node("SELECT");
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
      dNode = new node("DISTINCT");
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
      fromNode = new node("FROM");
      parent->subTree.push_back(fromNode);
      tablelist(parent);
    }
    else{
    }
    return;
}
