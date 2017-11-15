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
    read(dataTypeBuf, false);
    if (strcmp(dataTypeBuf, "INT") == 0) {
        crTableObj->field_types.push_back(INT);
    } else if (strcmp(dataTypeBuf, "STR20") == 0) {
        crTableObj->field_types.push_back(STR20);
    } else {
    }
}

void attributeTypeList(createTableData *crTableObj) {
    char *attrNameBuf;
    attrNameBuf = (char *)malloc(20*sizeof(char));
    attrName(attrNameBuf);
    crTableObj->field_names.push_back(string(attrNameBuf));
    dataType(crTableObj);
    char *comBuf = (char *)malloc(2*sizeof(char));
    read(comBuf, true);
    if (strcmp(comBuf, ",") == 0) {
        attributeTypeList(crTableObj);
    }
    return;
}
    
void createTable(createTableData *crTableObj) {
    char *tableNameBuf = (char*)malloc(10*sizeof(char));
    tableName(tableNameBuf);
    crTableObj->relationName = tableNameBuf;
    char *parenBuf;
    parenBuf = (char *)malloc(2*sizeof(char));
    read(parenBuf, true);
    if (strcmp(parenBuf, "(") == 0) {
        attributeTypeList(crTableObj);
    } else {
        cout << "Error in CREATE TABLE statement" << endl;
    }
    read(parenBuf, true);
    if (strcmp(parenBuf, ")") == 0) {
    }
}


