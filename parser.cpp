#include <iostream>
using namespace std;
#include <cstdlib>
#include <cstring>
#include "io.h"
//#include "common.h"
#include "insert.h"
#include "select.h"
#include "parseTree.h"

int main() {
    node *root;
    root = new node("ParserRoot", false);
    /* read the first word to check the type of statement */
    char *stmtBuf;
    stmtBuf = (char *)malloc(10*sizeof(char));
    read(stmtBuf, false);

    /* call the corresponding subroutine */
  //  cout << stmtBuf;
    if (strcmp(stmtBuf, "SELECT") == 0) {
       selectStmt(root);

  //  cout << "print select: " << root->subTree[0]->termString <<endl;
      // cout << "print DISTINCT: " << root->subTree[1]->termString <<endl;
  //     cout << "print star" << root->subTree[1]->subTree[0]->termString <<endl;
  //   cout << "select col print: " << root->subTree[1]->subTree[0]->subTree[0]->subTree[0]->termString << endl;
//   cout << "select comma print: " << root->subTree[1]->subTree[0]->subTree[1]->termString << endl;
//     cout << "select col print: " << root->subTree[1]->subTree[0]->subTree[2]->subTree[0]->subTree[0]->termString << endl;
// cout << "from tablename print" << root->subTree[3]->subTree[0]->subTree[0]->termString << endl;
 //cout << "from comma print" << root->subTree[3]->subTree[1]->termString << endl;
 //cout << "from tablename print" << root->subTree[3]->subTree[2]->subTree[0]->subTree[0]->termString << endl;

    }
    else if (strcmp(stmtBuf, "INSERT") == 0){
      insertStmt(root);
    }
}
