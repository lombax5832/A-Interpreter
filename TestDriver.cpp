using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {

  string input = "";
  string test = "(abc abc";

  ACode code;
  //cout << code.infixToPostfix(test) << endl;
  cout << code.resolveIdensInExpression(test) << endl;
  //code.fromText(test);
  code.printLines();

  return 0;
}