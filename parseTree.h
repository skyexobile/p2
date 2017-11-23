#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <string>
#include <vector>
using namespace std;

class node {
    public:
    string nodeType;
    vector<node *> subTree;

    node(string strNodeType) {
        nodeType = strNodeType;
    }
};

#endif
