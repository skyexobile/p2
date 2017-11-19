#include "common.h"
#include <string>
#include <cstring>
#include<iostream>
#include<iomanip>
using namespace std;
#include<vector>
#include "parseTree.h"
#include "io.h"
#include "stmtDataStructs.h"
#define WIDTH 15
void tableName(char *tableNameBuf);

void columnName(char* colNameBuf){
  cout << "columnName" << endl;
    char *temp, *tableName, *attrName;
    readWord(colNameBuf);
    if(readPeriod()){
      strcat(colNameBuf, ".");
      readWord(temp);
      strcat(colNameBuf, temp);
    }
    return;
}
void attrName(char *attrNameBuf){
    readWord(attrNameBuf);
    if (!attrNameBuf[0]) {
        cerr << "attrName: attrName not read";
    }
    return;
}

void tableName(char *tableNameBuf){
  readWord(tableNameBuf);
    if (!tableNameBuf[0]) {
        cerr << "tableName: tableName not read";
    }
  return;
}

void term(node* parent){
  node* termNode, *term, *other;
  termNode = new node("term", false);
  char *temp = (char*)malloc(40*sizeof(char));
  readWord(temp);
  if(strcmp(temp, "\"")==0){
    readWord(temp);
    term = new node(temp, true);
    termNode->subTree.push_back(term);
    readWord(temp);
    return;
  }
  readWord(temp);
  if(isdigit(temp[0])){
    term = new node(temp, true);
    termNode->subTree.push_back(term);
    return;
  }
  //this is a columname
  columnName(temp);
  return;
}
void expression(node* parent){
  node *expressNode, *other;
  bool flag = false;
  expressNode = new node("expression", false);
  parent->subTree.push_back(expressNode);
  //check if there's a (  for +, =, or * after term
  char *temp = (char*)malloc(10*sizeof(char));
  readWord(temp);
  if(strcmp(temp, "(" )==0){
    other = new node("(", true);
    expressNode->subTree.push_back(other);
    flag = true;
  }

  term(expressNode);
  if(flag){
    //next should be +, -, or *
    readWord(temp);
    other = new node(temp, true);
    expressNode->subTree.push_back(other);
    term(expressNode);
    readWord(temp);
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
  readWord(temp);

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
  readWord(word);
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
  readWord(word);
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
