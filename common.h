#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "parseTree.h"

void tableName(node* parent);
void attrName(node *parent);
void columnName(node *parent);
void printTree(node *curr, int indent);

void term(node* parent);
void expression(node* parent);
void comp_op(node* parent);
void booleanFactor(node* parent);
void booleanTerm(node* parent);
void searchCondition(node* parent);

#endif
