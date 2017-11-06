#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"


void value(node* parent){
  node* valueNode, *value;
  valueNode = new node();
  parent->subTree.push_back(valueNode);
  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  read(temp, true);
  value = new node(temp);
  valueNode->subTree.push_back(value);
  return;

}
void valueList(node* parent){
  node* valuelistNode, *commaNode;
  valuelistNode = new node();
  parent->subTree.push_back(valuelistNode);
  value(valuelistNode);
  char *temp;
  temp = (char *)malloc(20*sizeof(char));
  read(temp, true);
  if(temp[0] == ","){
    commaNode = new node(",");
    valuelistNode -> subTree.push_back(commaNode);
    valueList(valueListNode);
  }
  else if(temp[0] == ")"){
    putChar(temp);
  }
  return;
}

void insertTuples(node* parent){
  node *insertTuplesNode, *LPNode, *RPNode;
  insertTuplesNode = new node();
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
    if(temp[0] == "("){
      LPNode = new node("(");
      parent->subTree.push_back(LPNode);
      valuelist(insertTuplesNode);
    }
    if(temp[0] == ")"){
      RPNode = new node(")");
      parent->subTree.push_back(RPNode);
    }
  }
    return;


}


void attrName(node *parent){
    node *attrNameNode, *commaNode;
    attrNameNode = new node();
    parent->subTree.push_back(attrNameNode);
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    read(temp, false);
    node *atrName;
    atrName = new node(temp);
    attrNameNode -> subTree.push_back(atrName);
    read(temp, true);
    if(temp[0] ==  ')'){
      putchar(temp[0]);
      return;
    }
    else if (temp[0] == ",") {
        commaNode = new node(",");
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
    attrListtNode = new node();
    parent -> subTree.push_back(attrListNode);
    attrName(attrListNode);

    return;
}

//use tablename function
void insertTable(node *parent) {
  cout << "tableName " << endl;
    node *tblNameNode, *tblName, *LPNode, *RPNode;

    tblNameNode = new node();
    parent -> subTree.push_back(tblNameNode);
    char* c= (char *)malloc(20*sizeof(char));
    read(c, false);
    tblName = new node(c);
    parent->subTree.push_back(tblName);

    read(c, true);
    if(c[0] == '\0'){
      read(c,true);
    }
    if(strcmp(c, "(") == 0){
      //append and call attribute list
      LPNode = new node("(");
      parent->subTree.push_back(LPNode);
      attrList(parent);
    }
    if(strcmp(c, ")") == 0){
      //append and call attribute list
      RPNode = new node(")");
      parent->subTree.push_back(LPNode);
    }

    return;

}

void insertStmt(node *parent) {
    cout << "in insert Stmt" << endl;
    node *insertNode, *intoNode;
    insertNode = new node("INSERT");
    parent -> subTree.push_back(insertNode);
    //check if next word is "INTO"
    char* c= (char *)malloc(10*sizeof(char));
    read(c, true);
    if(strcmp(c, "INTO") ==0){
      intoNode = new node("INTO");
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
