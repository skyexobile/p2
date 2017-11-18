#include <iostream>
#include <stack>
#include <ctype.h>
#include <cstdlib>

using namespace std;

static stack<char> buffer;
static int flag;
char getChar1() {
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
        c = getChar1();
        flag = 1;
    }
    return c;
}

char getChar() {
    char c;
    if (!buffer.empty()) {
        c = buffer.top();
        buffer.pop();
    } else {
        cin >> c;
    }
    return c;
}

void putChar(char c) {
    buffer.push(c);
}

void readWord(char *sbuf) {
    char c;
    cin.sync_with_stdio(false);

    cin >> c;
    for (int i=0;; i++) {
        if (isalpha(c) || isdigit(c)) {
            sbuf[i] = c;
        } else {
          if(c==' '){
            sbuf[i] = '\0';
            return;
          }
          if(c=='\0'){
            sbuf[i] = '\0';

            return;
          }
            //at this point, we're going to find character at end of word, like a comma..so we want to push it back into buffer
            cin.putback(c);
            sbuf[i] = '\0';
            return;
        }
        cin.get(c);
    }
}

void readQuote() {
    char c;
    cin >> c;
    if ((c != '"') && (c != '\'')) {
        cin.putback(c);
    }
}

bool readParen() {
    char c;
    cin >> c;
    if ((c == '(') || (c == ')')) {
        return true;
    } else {
        cin.putback(c);
        cin >> c;
        return false;
    }
}

bool readStar() {
    char c;
    cin >> c;
    if (c == '*') {
        return true;
    } else {
        cin.putback(c);
        return false;
    }
}

bool readComma() {
    char c;
    cin >> c;
    if (c == ',') {
        return true;
    } else {
        cin.putback(c);
        return false;
    }
}

bool readPeriod() {
    char c;
    cin >> c;
    if (c == '.') {
        return true;
    } else {
        cin.putback(c);
        return false;
    }
}

char readCompOp() {
    char c;
    cin >> c;
    if ((c == '<') || (c == '>') ||  (c == '=')){
        return c;
    } else {
        cin.putback(c);
        return '\0';
    }

}
/*
int main() {
    char *ptr;
    ptr = (char *)malloc(10*sizeof(char));
    for(int i = 0; i<4; i++)
    {
      readWord(ptr);
      cout << string(ptr) << endl;
    }

}*/
