#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "stmtDataStructs.h"
#include "parseTree.h"

void tableName(char *tableNameBuf);
void attrName(char *attrNameBuf);
void columnName(node *parent);
void printTree(node *curr, int indent);

void term(node* parent);
void expression(node* parent);
void comp_op(node* parent);
void booleanFactor(node* parent);
void booleanTerm(node* parent);
void searchCondition(node* parent);

#endif
