#ifndef SELECT_H
#define SELECT_H

#include "stmtDataStructs.h"
void columnName(selectData *selDataObj);
void selSublist(selectData *selDataObj);
void selList(selectData *selDataObj);
node* selectStmt(selectData *selDataObj);

#endif
