#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"

void tableName1(node *parent){
  node *tableNameNode, *tblNode;
  tableNameNode = new node();
  parent->subTree.push_back(tableNameNode);
  char *word = (char*)malloc(10*sizeof(char));
  read(word, false);
  tblNode = new node(word);
  tableNameNode->subTree.push_back(tblNode);
  return;
}

void createTable(node *parent) {
    cout << "in createTable << endl;
    node *createNode, *tableNode, *tableNameNode;
    createNode = new node("CREATE");
    tableNode = new node("TABLE");
    parent -> subTree.push_back(createNode);
    parent -> subTree.push_back(tableNode);
    tableName1(


}


