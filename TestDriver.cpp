using namespace std;

#include<string>
#include<iostream>
#include<sstream>

#include"App.h"

int main() {

  string input = "";

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