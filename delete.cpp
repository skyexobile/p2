#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include <string>
#include "io.h"
#include "stmtDataStructs.h"
#include "common.h"
#include "searchCondition.h"
node * deleteStmt(deleteData *delDataObj){
    char* c= (char *)malloc(10*sizeof(char));
    readWord(c);
    //next word should be FROM
    if(strcmp(c, "FROM") ==0){
      char *tableNameBuf = (char*)malloc(10*sizeof(char));
      tableName(tableNameBuf);
      delDataObj->relation_name=(string(tableNameBuf));
        readWord(c);
        if(strcmp(c, "WHERE") == 0) {
            string searchStrBuf;
            node *root;
            getline(cin, searchStrBuf);
            root = createTree(searchStrBuf);
            return root;
        }
        for(int i = strlen(c)-1; i >=0; i--){
            cin.putback(c[i]);
        }
        // select statement without the where clause
        return nullptr;
    }
  }
