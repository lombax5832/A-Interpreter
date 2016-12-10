using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {
  string line = "var you";

  ALine line2(10, line);
  string input = "";
  string test = "(abc+2)/2";

  ACode code;
  //cout << code.infixToPostfix(test) << endl;
  //cout << code.infixToPostfix(code.resolveIdensInExpression(test, 0, test.size())) << endl;
  //cout << code.evalPostFix(code.infixToPostfix(code.resolveIdensInExpression(test, 0, test.size())), 1) << endl;
  //cout << code.resolveIdensInExpression(test, 0, test.size()) << endl;
  //code.fromText(test);
  code.printLines();

  cout << code.isVarStatementValid(line2) << endl;

  return 0;
}