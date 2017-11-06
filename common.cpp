#include "common.h"
#include <string>
#include<iostream>
#include<vector>
void printTree(node* curr) {
    string termStr;
    termStr = curr -> termString;
    if (!termStr.empty()) {
        // terminal node
        cout << termStr << '\t';
        return;
    } else {
        // non-terminal node
        vector<node *> vectChildNodes;
        vectChildNodes = curr -> subTree;
        for (auto& nodeptr:vectChildNodes) {
            cout << nodeptr << endl;
            printTree(nodeptr);
        }
        return;
    }
}
