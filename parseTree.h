#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <string>
#include <vector>
using namespace std;

class node {
    public:
    string termString;
    vector<node *> subTree;

    node() {}
    node(string nodeName) {
        termString = nodeName;
    }
};

#endif
