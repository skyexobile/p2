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
        cin.get(c);
        flag = 1;
    }
    return c;
}

void putChar(char c) {
    buffer.push(c);

}

void read(char *sbuf, bool oneChar) {
    char c = (getChar());
    switch(c) {
        case '*':
        case ',':
        case '.':
        case '\'':
        case '\n':
        case '(':
        case ')':
        case '<':
        case '>':
        case '=':
        case '\"':
            sbuf[0] = c;
            sbuf[1] = '\0';
            break;
        case ' ':
            sbuf[0] = '\0';
            break;
        default:
            // we read in a letter or a digit, if we were expecting a parentheses or something, then we would have to return null
            putChar(c);
            sbuf[0] = '\0';
            break;
    }

    // we haven't read a * , ' \n or .
    if (oneChar) {
        return;
    }

    for (int i=0; c = getChar(); i++) {
        if (isalpha(c) || isdigit(c)) {
            sbuf[i] = c;
        } else {
            //at this point, we're going to find character at end of word, like a comma..so we want to push it back into buffer
            buffer.push(c);
            sbuf[i] = '\0';
            return;
        }
    }
}
/*
int main() {
    char *ptr;
    ptr = (char *)malloc(10*sizeof(char));
    read(ptr, false);
    cout << ptr;
    read(ptr, true);
    cout << ptr;
    return 0;

}*/
