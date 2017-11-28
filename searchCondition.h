#ifndef SEARCHCOND_H
#define SEARCHCOND_H

#include <string>
#include "parseTree.h"
#include "Tuple.h"


string switchDelimiter(string delimiter);
void recursiveSplit(node *parent, vector<string> const &tokens, string delimiter);
node *createTree(char *searchStrBuf);
int hashString(string str);
int evalArith(node *root, Tuple selTuple);
bool evalBool(node *root, Tuple selTuple);
#endif
