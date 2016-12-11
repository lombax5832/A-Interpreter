using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"App.h"

int main() {
  string line = "if (toasty) goto32";

  ALine line2(10, line);
  string input = "";
  string test = "10 var test = 40;11 test = test-1;12 print test;16 if (35-test) goto 11;20 stop;";

  ACode code;
  
  cout << "Enter \"kb\" to enter lines from keyboard, "
    << "or \"file\" to execute file from file.txt" << endl;
  cin >> input;
  if (input == "kb") {
    cin.clear();
    code.fromKeyboard();
  } else if (input == "file") {
    code.fromFile("file.txt", true);
  } else {
    cout << "Invalid text entered!" << endl;
    exit(2);
  }
  cout << endl << "--- A++ Code being Executed ---" << endl;
  code.printLines();
  cout << "-------------------------------" << endl << endl;

  code.executeCode();

  return 0;
}