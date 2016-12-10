using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {

  string input = "";
  string test = "(-abc +abc) + 3-1";

  ACode code;
  //cout << code.infixToPostfix(test) << endl;
  cout << code.infixToPostfix(code.resolveIdensInExpression(test, 0, test.size())) << endl;
  //cout << code.resolveIdensInExpression(test, 0, test.size()) << endl;
  //code.fromText(test);
  code.printLines();

  return 0;
}