using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"ACode.h"

int main() {
  string line = "if (toasty) goto32";

  ALine line2(10, line);
  string input = "";
  string test = "1 print 30;10 var test = 30;11 print test;12 test = test+1;13 print test;16 if (0) goto 11;20 stop;";

  ACode code;

  code.fromText(test);
  code.printLines();
  cout << endl;
  code.scanLines();
  ALine lineT(code.firstLine());
  code.executeCode(lineT);

  return 0;
}