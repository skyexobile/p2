#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include "stmtDataStructs.h"
#include "parseTree.h"
#include "Block.h"
#include "Config.h"
#include "Disk.h"
#include "Field.h"
#include "MainMemory.h"
#include "Relation.h"
#include "Schema.h"
#include "SchemaManager.h"
#include "Tuple.h"

void tableName(char *tableNameBuf);
void attrName(char *attrNameBuf);
void columnName(char* colNameBuf);
void printTree(node *curr, int indent);

void term(node* parent);
void expression(node* parent);
void comp_op(node* parent);
void booleanFactor(node* parent);
void booleanTerm(node* parent);
void searchCondition(node* parent);

#endif
