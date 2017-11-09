#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"
#include "common.h"
#include <cstring>

void attributeTypeList(node *parent) {
    
void createTable(node *parent) {
    cout << "in createTable" << endl;
    node *createNode, *tableNode, *tableNameNode;
    createTableNode = new node("CREATE TABLE");
    parent -> subTree.push_back(createTableNode);
    tableName(parent);
    char *tableNameBuf, *parenBuf;
    tableNameBuf = (char *)malloc(20*sizeof(char));
    read(tableNameBuf, false);
    parenBuf = (char *)malloc(2*sizeof(char));
    read(parenBuf, true);
    if (strcmp(parenBuf, '(') == 0) {
        attributeTypeList(parent);
    } else {
        cout << "Error in CREATE TABLE statement" << endl;
    }
    read(parenBuf, true);
    if (strcmp(parenBuf, ')') == 0) {
        cout << "create table read" << endl;
    }
}


