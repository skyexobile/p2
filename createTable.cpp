#include <iostream>
using namespace std;
#include <cstdlib>
#include "io.h"
#include "parseTree.h"
#include "common.h"
#include <cstring>
#include "stmtDataStructs.h"

void dataType(createTableData *crTableObj) {
    char *dataTypeBuf = (char *)malloc(10*sizeof(char));
    readWord(dataTypeBuf);
    if (strcmp(dataTypeBuf, "INT") == 0) {
        crTableObj->field_types.push_back(INT);
    } else if (strcmp(dataTypeBuf, "STR20") == 0) {
        crTableObj->field_types.push_back(STR20);
    } else {
        cerr << "dataType: datatype is not STR20 or INT"; 
    }
}

void attributeTypeList(createTableData *crTableObj) {
    char *attrNameBuf;
    attrNameBuf = (char *)malloc(20*sizeof(char));
    attrName(attrNameBuf);
    crTableObj->field_names.push_back(string(attrNameBuf));
    dataType(crTableObj);
    if (readComma()) {
        attributeTypeList(crTableObj);
    }
    return;
}
    
void createTable(createTableData *crTableObj) {
    char *tableNameBuf = (char*)malloc(10*sizeof(char));
    tableName(tableNameBuf);
    crTableObj->relationName = tableNameBuf;
    if (readParen()) {
        attributeTypeList(crTableObj);
    } else {
        cerr << "createTable: missing left parentheses";
    }
    if (!readParen()) {
        cerr << "createTable: missing right parentheses";
    }
}


