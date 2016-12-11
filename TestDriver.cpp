using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {
  string line = "if (toasty) goto32";

  ALine line2(10, line);
  string input = "";
  string test = "10 var test = 3000;11 test = test-1;12 print test;16 if (35-test) goto 11;20 stop;";

  ACode code;

  code.fromText(test);
  code.printLines();
  cout << endl;
  code.scanLines();
  ALine lineT(code.firstLine());
  code.executeCode(lineT);

  return 0;
}