#include "common.h"
#include <string>
#include <cstring>
#include<iostream>
#include<iomanip>
using namespace std;
#include<vector>
#include "parseTree.h"
#include "io.h"
const int WIDTH = 15;

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
        putChar(',');
    }

    return;
}
void attrName(node *parent){
    node *attrNameNode, *atrName;
    attrNameNode = new node("attributeName", false);
    parent->subTree.push_back(attrNameNode);
    cout << "ADDED  attribute name node" << endl;
    char *temp;
    temp = (char *)malloc(20*sizeof(char));
    read(temp, false);
    atrName = new node(temp, true);
    attrNameNode -> subTree.push_back(atrName);
    return;
}

void tableName(node *parent){
  node *tableNameNode, *tblNode;
  tableNameNode = new node("tableName", false);
  parent->subTree.push_back(tableNameNode);
  char *word = (char*)malloc(10*sizeof(char));
  read(word, false);
  tblNode = new node(word, true);
  tableNameNode->subTree.push_back(tblNode);
  return;
}
void printTree(node* curr, int indent) {
    bool isTermNode;
    isTermNode = curr->isTermNode;
    if (isTermNode) {
        // terminal node
        cout << setw(WIDTH*indent) << curr->nodeName << endl;
        return;
    } else {
        // non-terminal node
        cout << setw(WIDTH*indent) << curr->nodeName << endl;
        vector<node *> vectChildNodes;
        vectChildNodes = curr -> subTree;
        for (auto& nodeptr:vectChildNodes) {
            printTree(nodeptr, indent+1);
        }
      }
        return;
    }
void term(node* parent){
  node* termNode, *term, *other;
  termNode = new node("term", false);
  char *temp = (char*)malloc(40*sizeof(char));
  read(temp, true);
  if(strcmp(temp, "\"")==0){
    read(temp, false);
    term = new node(temp, true);
    termNode->subTree.push_back(term);
    read(temp, true);
    return;
  }
  read(temp,false);
  if(isdigit(temp[0])){
    term = new node(temp, true);
    termNode->subTree.push_back(term);
    return;
  }
  //this is a columname
  columnName(termNode);
  return;
}
void expression(node* parent){
  node *expressNode, *other;
  bool flag = false;
  expressNode = new node("expression", false);
  parent->subTree.push_back(expressNode);
  //check if there's a (  for +, =, or * after term
  char *temp = (char*)malloc(10*sizeof(char));
  read(temp, true);
  if(strcmp(temp, "(" )==0){
    other = new node("(", true);
    expressNode->subTree.push_back(other);
    flag = true;
  }

  term(expressNode);
  if(flag){
    //next should be +, -, or *
    read(temp, true);
    other = new node(temp, true);
    expressNode->subTree.push_back(other);
    term(expressNode);
    read(temp, true);
    if(strcmp(temp, ")") ==0){
      other = new node(")", true);
      expressNode->subTree.push_back(other);
    }
  }
  return;
}
void comp_op(node* parent){
  node *comp, *comp_operator;
  bool flag = false;
  comp = new node("comp_operation", false);
  parent->subTree.push_back(comp);
  char *temp = (char*)malloc(3*sizeof(char));
  read(temp, true);

  if(strcmp(temp, "<") == 0){
    comp_operator = new node("<", true);
    flag = true;
  }
  else if(strcmp(temp, ">") == 0){
      comp_operator = new node(">", true);
      flag = true;
  }
  else if(strcmp(temp, "=") == 0){
      comp_operator = new node("=", true);
      flag = true;
  }
  if(flag){
    comp->subTree.push_back(comp_operator);
  }
  return;
}
void booleanFactor(node* parent){
  node* boolFactor;
  boolFactor = new node("boolFactor", false);
  parent->subTree.push_back(boolFactor);
  expression(boolFactor);
  comp_op(boolFactor);
  expression(boolFactor);

}
void booleanTerm(node* parent){
  node *boolTerm, *other;
  boolTerm = new node("booleanTerm", false);
  parent->subTree.push_back(boolTerm);
  booleanFactor(boolTerm);
  //check if there's an AND afterwards
  char *word = (char*)malloc(10*sizeof(char));
  read(word, false);
  if(strcmp(word, "AND") ==0){
    other = new node("AND", true);
    boolTerm->subTree.push_back(other);
    booleanTerm(boolTerm);
  }
  else{
    //putting what we read back
    for(int i =(strlen(word)-1); i>=0; i--){
      putChar(word[i]);
      return;
    }
  }
}

void searchCondition(node *parent)
{

  node *searchCond, *other;
  searchCond = new node("searchCondition", false);
  parent->subTree.push_back(searchCond);
  booleanTerm(searchCond);
  //check if there's an OR after it
  char *word = (char*)malloc(10*sizeof(char));
  read(word, false);
  if(strcmp(word, "OR") ==0){
    other = new node("OR", true);
    searchCond->subTree.push_back(other);
    searchCondition(searchCond);
  }
  else{
    //putting what we read back
    for(int i =(strlen(word)-1); i>=0; i--){
      putChar(word[i]);
      return;
    }
  }
}
