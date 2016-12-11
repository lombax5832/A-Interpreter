using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {
  string line = "if (toasty) goto32";

  ALine line2(10, line);
  string input = "";
  string test = "10 var test = 30;20 if (yes) goto 31;31 print test;33 test = 23;34 stop;";

  ACode code;
  //cout << code.infixToPostfix(test) << endl;
  //cout << code.infixToPostfix(code.resolveIdensInExpression(test, 0, test.size())) << endl;
  //cout << code.evalPostFix(code.infixToPostfix(code.resolveIdensInExpression(test, 0, test.size())), 1) << endl;
  //cout << code.resolveIdensInExpression(test, 0, test.size()) << endl;
  code.fromText(test);
  code.printLines();
  cout << endl;
  code.scanLines();

  return 0;
}