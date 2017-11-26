#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#include "parseTree.h"
#include "common.h"

string switchDelimiter(string delimiter) {
    char delim;
    if (delimiter == "OR") {
        delim = '|';
    } else if (delimiter == "AND") {
        delim = '&';
    } else {
        delim = delimiter[0];
    }

    switch(delim) {
        case '|':
            return "AND";
        case '&':
            return "<";
        case '<':
            return ">";
        case '>':
            return "=";
        case '=':
            return "+";
        case '+':
            return "-";
        case '-':
            return "*";
        case '*':
            return " ";
    }
}

void recursiveSplit(node *parent, vector<string> const &tokens, string delimiter) {
    if (tokens.size() == 1) {
        node *mathNode;
        mathNode = new node(tokens[0]);
        parent->subTree.push_back(mathNode);
        return;
    } else {
        vector<string>::const_iterator it;
        for (it = tokens.begin(); it != tokens.end(); it++) {
            if (*it == delimiter) {
                break;
            }
        }
        if (it != tokens.end()) {
            // delimiter was found
            node *delimNode;
            delimNode = new node(delimiter);
            parent->subTree.push_back(delimNode);
            vector<string> leftTokens(tokens.begin(), it), rightTokens(it+1, tokens.end());
            string newDelimiter = switchDelimiter(delimiter);
            recursiveSplit(delimNode, leftTokens, newDelimiter);
            recursiveSplit(delimNode, rightTokens, delimiter);
        } else {
            // delimiter is not found
            string newDelimiter = switchDelimiter(delimiter);
            recursiveSplit(parent, tokens, newDelimiter);
        }
    }
}

node *createTree(char *searchStrBuf) {
    string buf;
    string str = searchStrBuf;
    stringstream ss(str);
    vector<string> tokens;
    while(ss >> buf) {
        if ((buf != ")") && (buf != "(")) {
            tokens.push_back(buf);
        }
    }
    node *root = new node("searchTreeRoot");
    recursiveSplit(root, tokens, "OR");
    return root;
}

int main() {
    char *buf;
    node *root;
    buf = (char *)malloc(50*sizeof(char));
    fgets(buf,50,stdin);
    root = createTree(buf);
    printTree(root, 0);
}


