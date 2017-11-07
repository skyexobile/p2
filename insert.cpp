#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"
#include "select.h"
#include "common.h"

void value(node* parent){
  node* valueNode, *value;
  valueNode = new node("value", false);
  parent->subTree.push_back(valueNode);
  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  read(temp, true);
  value = new node(temp, true);
  valueNode->subTree.push_back(value);
  return;

}
void valueList(node* parent){
  node* valuelistNode, *commaNode;
  valuelistNode = new node("valuelist", false);
  parent->subTree.push_back(valuelistNode);
  value(valuelistNode);
  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  read(temp, true);
  if(strcmp(temp, ",") ==0){
    commaNode = new node(",", true);
    valuelistNode -> subTree.push_back(commaNode);
    valueList(valuelistNode);
  }
  else if(strcmp(temp,")") == 0){
    putChar(')');
  }
  return;
}

void insertTuples(node* parent){
  node *insertTuplesNode, *LPNode, *RPNode;
  insertTuplesNode = new node("insertTuples", false);
  parent->subTree.push_back(insertTuplesNode);
  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  read(temp, false);
  if(strcmp(temp, "SELECT") == 0){
    selList(insertTuplesNode);
    return;
  }
  else if(strcmp(temp, "VALUES") == 0){
    //insert value list or select statement
    read(temp, true);
    if(temp[0] =='\0')
    {
      read(temp, true);
    }
    if(strcmp(temp, "(") == 0){
      LPNode = new node("(", true);
      parent->subTree.push_back(LPNode);
      valueList(insertTuplesNode);
    }
    if(strcmp(temp, ")" )== 0){
      RPNode = new node(")", true);
      parent->subTree.push_back(RPNode);
    }
  }
    return;


}

void attrList(node*);
void attrName(node *parent){
    node *attrNameNode, *commaNode;
    attrNameNode = new node("attrName", false);
    parent->subTree.push_back(attrNameNode);
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    read(temp, false);
    node *atrName;
    atrName = new node(temp, true);
    attrNameNode -> subTree.push_back(atrName);
    read(temp, true);
    if(strcmp(temp, ")") == 0){
      putChar(')');
      return;
    }
    else if (strcmp(temp, ",") ==0){
        commaNode = new node(",", true);
        parent -> subTree.push_back(commaNode);
        attrList(parent);
    }
    return;
}

void attrList(node *parent) {
  cout << "attribute list" << endl;
    node *attrListNode, *commaNode;
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    attrListNode = new node("attrList", false);
    parent -> subTree.push_back(attrListNode);
    attrName(attrListNode);

    return;
}

//use tablename function
void insertTable(node *parent) {
  cout << "tableName " << endl;
    node *tblNameNode, *tblName, *LPNode, *RPNode;

    tableName(parent);
    char *c;
    c = (char *)malloc(20*sizeof(char));
    read(c, true);
    if(c[0] == '\0'){
      read(c,true);
    }
    if(strcmp(c, "(") == 0){
      //append and call attribute list
      LPNode = new node("(", true);
      parent->subTree.push_back(LPNode);
      attrList(parent);
    }
    if(strcmp(c, ")") == 0){
      //append and call attribute list
      RPNode = new node(")", true);
      parent->subTree.push_back(LPNode);
    }

    return;

}

void insertStmt(node *parent) {
    cout << "in insert Stmt" << endl;
    node *insertNode, *intoNode;
    insertNode = new node("INSERT", true);
    parent -> subTree.push_back(insertNode);
    //check if next word is "INTO"
    char* c= (char *)malloc(10*sizeof(char));
    read(c, true);
    if(strcmp(c, "INTO") ==0){
      intoNode = new node("INTO", true);
      parent->subTree.push_back(intoNode);
    }
    else{
      //something is wrong
    return;
    }
    insertTable(parent);
    insertTuples(parent);


    return;
}
