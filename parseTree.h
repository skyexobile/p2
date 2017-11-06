#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <string>
#include <vector>
using namespace std;

class node {
    public:
    string nodeName;
    bool isTermNode;
    vector<node *> subTree;

   // node() {}
    node(string strNodeName, bool isTerm) {
        nodeName = strNodeName;
        if (isTerm) {
            isTermNode = true;
        } else {
            isTermNode = false;
        }
    }
};

#endif
