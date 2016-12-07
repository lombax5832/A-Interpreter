using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {

  string input = "";
  string test = "32 goto test 34;33 hello world;";

  ACode code;
  code.fromText(test);
  code.printLines();

  return 0;
}