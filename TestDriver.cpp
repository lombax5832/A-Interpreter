using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {

  string input = "";
  string test = "print abc";

  ACode code;
  cout << Statements[code.getStatementType(test)] << endl;
  //code.fromText(test);
  code.printLines();

  return 0;
}