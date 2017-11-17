#include <iostream>
#include <stack>
#include <ctype.h>
#include <cstdlib>
using namespace std;

static stack<char> buffer;
static int flag;
char getChar() {
    char c;
    char p = cin.peek();

  if(flag== 1 && p == '\n'){
    flag = 0;
      return '\n';
    }
    if (!buffer.empty()) {
        c = buffer.top();
        buffer.pop();
    } else {
        cin >> c;
        flag = 1;
    }
    return c;
}

void putChar(char c) {
    buffer.push(c);
}

void readWord(char *sbuf) {
    char c;
    cin >> c;
    
    for (int i=0;; i++) {
        if (isalpha(c) || isdigit(c)) {
            sbuf[i] = c;
        } else {
            //at this point, we're going to find character at end of word, like a comma..so we want to push it back into buffer
            buffer.push(c);
            sbuf[i] = '\0';
            return;
        }
        cin.get(c);
    }
}

void readQuote() {
    char c;
    cin >> c;
    if ((c != '\"') || (c != '\'')) {
        putChar(c);
    }
}

void readParen() {
    char c;
    cin >> c;
    if ((c != '(') || (c != ')')) {
        putChar(c);
    }
}

bool readStar() {
    char c;
    cin >> c;
    if (c == '*') {
        return true;
    } else {
        putChar(c);
        return false;
    }
}

bool readComma() {
    char c;
    cin >> c;
    if (c == ',') {
        return true;
    } else {
        putChar(c);
        return false;
    }
}

bool readPeriod() {
    char c;
    cin >> c;
    if (c == '.') {
        return true;
    } else {
        putChar(c);
        return false;
    }
} 

char readCompOp() {
    char c;
    cin >> c;
    if ((c == '<') || (c == '>') ||  (c == '=')){
        return c;
    } else {
        putChar(c);
        return '\0';
    }
} 

int main() {
    char *ptr;
    ptr = (char *)malloc(10*sizeof(char));
    //bool b =readStar();
    readWord(ptr);
    cout << ptr;
    return 0;
}
