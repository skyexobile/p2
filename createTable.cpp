#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"
#include "common.h"
#include <cstring>

void dataType(node *parent) {
    node *dataTypeNode, *dataType;
    dataTypeNode = new node("dataTypeNode", false);
    parent->subTree.push_back(dataTypeNode);
    char *dataTypeBuf = (char *)malloc(10*sizeof(char));
    read(dataTypeBuf, false);
    if (strcmp(dataTypeBuf, "INT") == 0) {
        dataType = new node("INT", true);
    } else if (strcmp(dataTypeBuf, "STR20") == 0) {
        dataType = new node("STR20", true);
    } else {
    }
    dataTypeNode->subTree.push_back(dataType);
}

void attributeTypeList(node *parent) {
    node *attrTypeListNode;
    attrTypeListNode = new node("attributeTypeList", false);
    parent->subTree.push_back(attrTypeListNode);
    attrName(attrTypeListNode);
    dataType(attrTypeListNode);
    char *comBuf = (char *)malloc(2*sizeof(char));
    read(comBuf, true);
    if (strcmp(comBuf, ",") == 0) {
        node *comNode;
        comNode = new node(",",true);
        attrTypeListNode->subTree.push_back(comNode);
        attributeTypeList(attrTypeListNode);
    }
    return;
}
    
void createTable(node *parent) {
    cout << "in createTable" << endl;
    node *createTableNode, *tableNode, *tableNameNode;
    createTableNode = new node("CREATE TABLE", true);
    parent -> subTree.push_back(createTableNode);
    tableName(parent);
    char *parenBuf;
    parenBuf = (char *)malloc(2*sizeof(char));
    read(parenBuf, true);
    if (strcmp(parenBuf, "(") == 0) {
        node *LPNode;
        LPNode = new node("(", true);
        parent->subTree.push_back(LPNode);
        attributeTypeList(parent);
    } else {
        cout << "Error in CREATE TABLE statement" << endl;
    }
    read(parenBuf, true);
    if (strcmp(parenBuf, ")") == 0) {
        node *RPNode;
        RPNode = new node(")", true);
        parent->subTree.push_back(RPNode);
        cout << "create table read" << endl;
    }
}


