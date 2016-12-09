using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {

  string input = "";
  string test = "(2234 +-345) *322 /-3 + (30-23)#";

  ACode code;
  //cout << code.infixToPostfix(test) << endl;
  //code.fromText(test);
  code.printLines();

  return 0;
}