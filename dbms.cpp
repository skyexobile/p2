#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include "io.h"
#include "common.h"
#include "select.h"
#include "insert.h"
#include "createTable.h"
#include "parseTree.h"
#include "stmtDataStructs.h"

int main() {
    node *root;
    root = new node("parserRoot",false);
    /* read the first word to check the type of statement */
    char *stmtBuf;
    stmtBuf = (char *)malloc(10*sizeof(char));
    read(stmtBuf, false);

    /* call the corresponding subroutine */
    if (strcmp(stmtBuf, "SELECT") == 0) {
      // selectStmt(root);
    }
    else if (strcmp(stmtBuf, "INSERT") == 0){
        read(stmtBuf, false);
        if (strcmp(stmtBuf, "INTO") == 0) {
          insertData inDataObj;
          insertStmt(&inDataObj);
          cout << inDataObj << endl;        }
    }
    else if (strcmp(stmtBuf, "CREATE") == 0){
        read(stmtBuf, false);
        if (strcmp(stmtBuf, "TABLE") == 0){
            createTableData crTableObj;
            createTable(&crTableObj);
            cout << crTableObj << endl;
        }
    }
    // cout << endl << endl << "printing tree" << endl;
    // printTree(root, 0);
}
