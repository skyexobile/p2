#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#include <cstdlib>
#include "parseTree.h"
#include "common.h"
#include "io.h"
#include "Tuple.h"
#include "Schema.h"

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

node *createTree(const string& searchStrBuf) {
    string buf;
    stringstream ss(searchStrBuf);
    vector<string> tokens;
    while(ss >> buf) {
        if (buf == "ORDER") {
            break;
        }
        if ((buf != ")") && (buf != "(")) {
            tokens.push_back(buf);
        }
    }
    node *root = new node("searchTreeRoot");
    recursiveSplit(root, tokens, "OR");
    return root;
}

int hashString(string str) {
    int sum=0;
    for (auto it=str.begin(); it != str.end(); it++) {
        sum *= 10;
        sum += *it - '0';
    }
    return sum;
}

int evalArith(node *root, Tuple selTuple) {
    if (root->subTree.empty()) {
        string nodeStr = root->nodeType;
        char firstchar = nodeStr[0];
        if (isdigit(firstchar)) {
            return hashString(nodeStr);
        } else if (isalpha(firstchar)) {
            Schema tuple_schema = selTuple.getSchema();
            if (tuple_schema.getFieldType(nodeStr)==INT) {
                return selTuple.getField(nodeStr).integer;
            } else {
                return hashString(*selTuple.getField(nodeStr).str);
            }
        } else if (nodeStr[0] == '"') {
            string value = string(nodeStr.begin()+1, nodeStr.end()-1);
            return hashString(value);
        } else {
            cerr << "evalArith: Invalid value read";
        }
    } else {
        // We are sure that it is a binary operation
        int leftValue = evalArith(root->subTree[0], selTuple);
        int rightValue = evalArith(root->subTree[1], selTuple);
        if (root->nodeType == "+") {
            return leftValue + rightValue;
        } else if (root->nodeType == "-") {
            return leftValue - rightValue;
        } else if (root->nodeType == "*") {
            return leftValue * rightValue;
        } else {
            cerr << "evalArith: Invalid arithmetic operator";
        }
    }
}

bool evalBool(node *root, Tuple selTuple) {
    if (root->nodeType == "OR") {
        bool leftCond = evalBool(root->subTree[0], selTuple);
        if (leftCond) {
            return true;
        }
        bool rightCond = evalBool(root->subTree[1], selTuple);
        if (rightCond) {
            return true;
        }
        return false;
    } else if (root->nodeType == "AND") {
        bool leftCond = evalBool(root->subTree[0], selTuple);
        if (!leftCond) {
            return false;
        }
        bool rightCond = evalBool(root->subTree[1], selTuple);
        if (!rightCond) {
            return false;
        }
        return true;
    } else if (root->nodeType == "<") {
        int leftValue = evalArith(root->subTree[0], selTuple);
        int rightValue = evalArith(root->subTree[1], selTuple);
        if (leftValue < rightValue) {
            return true;
        }
        return false;
    } else if (root->nodeType == ">") {
        int leftValue = evalArith(root->subTree[0], selTuple);
        int rightValue = evalArith(root->subTree[1], selTuple);
        if (leftValue > rightValue) {
            return true;
        }
        return false;
    } else if (root->nodeType == "=") {
        int leftValue = evalArith(root->subTree[0], selTuple);
        int rightValue = evalArith(root->subTree[1], selTuple);
        if (leftValue == rightValue) {
            return true;
        }
        return false;
    }
}

/*
int main() {
    char *buf = (char *)malloc(100*sizeof(char));
    node *root;
    root = createTree(buf);
    printTree(root, 0);
    return 0;
}
*/   
