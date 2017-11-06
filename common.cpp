#include "common.h"
#include <string>
#include<iostream>
#include<iomanip>
using namespace std;
#include<vector>
#include "parseTree.h"

const int WIDTH = 20;


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
        return;
    }
}
