#include<iostream>
#include <cstdlib>
#include <stack>
using namespace std;

static stack<char> buffer;
char getChar() {
    char c;
    cin.get(c);
    return c;
}

void read(char *s, bool b) {
    char c;
    c = getChar();
    s[0] = c;
    s[1] = '\0';
}

int main() {
char c = (getChar());
  cout << "TEST" << endl;
  cout << "This is the character we got: "<< c << endl;

  switch(c) {
      cout << "switch!!!!!: "<< c << endl;
      case '*':

      case ',':
      case '.':
      case '\'':
      case '\n':

          cout << "added " << c << " into sbuf.. should be character " << endl;

      case ' ':
          break;
      default:
          // we read in a letter or a digit
          break;
        }
        return 0;
  }
