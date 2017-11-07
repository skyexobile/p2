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
  if(flag ==1 && p == '\n'){
      cout << " END OF stream" << endl;
      flag = 2;
      return '\0';
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
  if(flag == 2){
    if(buffer.empty()){
      cout << "empty buffer" << endl;
      sbuf[0] = '\0';
      return;
    }

  }
    char c = (getChar());
    cout << "This is the character we got: "<< c << endl;

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
            cout << "DETECTED A CHARACTER " << endl;
            sbuf[0] = c;
            sbuf[1] = '\0';
            cout << "added " << c << " into sbuf.. should be character " << endl;
            break;
        case ' ':
        case '\0':
        sbuf[0] = '\0';

           cout << "SPACE dtected" << endl;
           break;
        default:
            // we read in a letter or a digit
            putChar(c);
            break;
    }

    // we haven't read a * , ' \n or .
    if (oneChar) {
        cout << "true and returned?" << endl;
        return;
    }

    for (int i=0; c = getChar(); i++) {
        if (isalpha(c) || isdigit(c)) {
            sbuf[i] = c;
            cout << c << " is now in sbuf" << endl;
        } else {
          //at this point, we're going to find character at end of word, like a comma..so we want to push it back into buffer
          switch(c) {
              case '*':
              case ',':
              case '.':
              case '\'':
              case '(':
              case ')':
                  buffer.push(c);
                  cout << "added " << c << " into back into buffer..  " << endl;
                  return;
              case ' ':
              sbuf[i] = '\0';
            cout << "added null to sbuf " << endl;
            return;
          }
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
