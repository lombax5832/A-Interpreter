#include "App.h"

using namespace std;

#include<iostream>
#include<sstream>
#include<cmath>
#include<stack>
#include<fstream>

ACode::ACode() {
}

const ALine& ACode::firstLine() const {
  if (!lines.empty())
    return lines[0];
  cout << "ERROR NO LINES" << endl << endl;
  exit(2);
}

//Adds a line to lines vector if label is greater than previous label
void ACode::addline(const ALine line, bool strict) {
  //Preserves order to use binary search later
  if (lines.empty()) {
    lines.push_back(line);
    return;
  }
  vector<ALine>::iterator it = lines.begin();
  for (; it != lines.end(); it++) {
    if (line.label < it->label) {
      if (strict) {
        cout << "LINE " << line.label << ": ";
        cout << "ERROR, added label must be larger than previous labels." << endl;
        exit(2);
      }
      lines.insert(it, line);
      return;
    }
  }
  lines.push_back(line);
  //if (line.label > lines.back().label) {
  //  lines.push_back(line);
  //} else {
  //  cout << "LINE " << line.label << ": ";
  //  cout << "ERROR, added label must be larger than previous labels." << endl;
  //  exit(2);
  //}
}

void ACode::addVar(const AVar toAdd, const size_t label) {
  if (!doesVarExist(toAdd.iden)) {
    vars.push_back(toAdd);
  }
}

void ACode::modifyVar(const AVar toModify, const size_t label, bool strict) {
  vector<AVar>::iterator it = vars.begin();
  for (; it != vars.end(); it++) {
    if (it->iden == toModify.iden) {
      it->val = toModify.val;
      it->initialized = toModify.initialized;
      return;
    }
  }
  if (!strict) {
    addVar(toModify, label);
  } else {
    cout << "LINE " << label << ": ";
    cout << "ERROR, variable does not exist." << endl;
  }
}

//Using a large string, stores lines in lines vector
void ACode::fromText(const string &input) {
  size_t pos = 0;
  size_t lastPos = 0;

  pos = input.find(";", 0);

  while (pos != string::npos) {
    addline(textToLine(input, lastPos, pos), true);
    lastPos = pos + 1;
    pos = input.find(";", pos + 1);
  }
}

bool ACode::fromFile(const string & file, bool strict) {
  ifstream strm(file);
  string line = "";
  size_t pos = 0;

  if (!strm) {
    if (!strict)
      return false;
    cout << "Error, file could not be found." << endl;
    exit(2);
  }
  
  while (!strm.eof()) {
    getline(strm, line);
    pos = line.find(';', 0);
    if (pos == string::npos) {
      if (!strict) {
        strm.close();
        return false;
      }
      cout << "Error, lines must end in a \';\'" << endl;
      exit(2);
    }
    addline(textToLine(line, 0, pos), true);
  }
  strm.close();
  return true;
}

void ACode::fromKeyboard() {
  string line = "";
  size_t pos = 0;
  ALine tempLine(0, "");
  size_t tempLbl = 0;
  stringstream strm;

  cout << "-------- Keyboard A++ File Entry --------" << endl << endl;
  cout << "Enter Lines of A++ code below." << endl;
  cout << "Code will be saved to the file, fromKeyboard.txt." << endl;
  cout << "Enter \"clear\" with no line label to delete all lines" << endl;
  cout << "Enter \"del\" followed by a line label to delete line with given label"<<endl;
  cout << "Enter \"end\" with no line label when finished entering A++ code" << endl;

  if (!fromFile("fromKeyboard.txt", false)) {
    cout << "File does not exist, or errors exist." << endl;
    cout << "Starting with blank A++ code." << endl << endl;
  } else {
    cout << "Loaded code will be displayed below :" << endl << endl;
    printLines();
    cout << "-----------------------------------------" << endl << endl;
  }
  //cin >> line;
  while (true) {
    getline(cin, line);
    if (line == "") {
      continue;
    }
    if (line == "end") {
      break;
    }
    if (line == "clear") {
      lines.clear();
      cout << "All lines cleared" << endl;
      continue;
    }
    if (line.find("del ", 0) == 0) {
      strm << line.substr(4, line.size() - 4) + ' ';
      strm >> tempLbl;
      cout << tempLbl << endl;
      if (delLineAt(tempLbl)) {
        cout << "Line successfully deleted." << endl;
      } else {
        cout << "Line not found." << endl;
      }
      continue;
    }
    pos = line.find(';', 0);
    if (pos == string::npos) {
      cout << "Line must end in a \';\', try again." << endl;
      continue;
    }
    tempLine = textToLine(line, 0, pos);
    if (doesLineExist(tempLine.label)) {
      cout << "Line already exists at " << tempLine.label << endl;
      cout << "Enter \"del " << tempLine.label << "\" to remove it first" << endl;
    } else {
      addline(textToLine(line, 0, pos), false);
    }
  }
  toFile("fromKeyboard.txt");
}

//Prints all stored lines
void ACode::printLines() const {
  if (!lines.empty()) {
    for (size_t i = 0; i < lines.size(); i++) {
      cout << lines.at(i).label << ' ' << lines.at(i).line << endl;
    }
  }
}

//Converts range of a string into a line and stores in lines vector
ALine ACode::textToLine(const string& input, const size_t begin, const size_t end) const{
  size_t label = 0;
  string line = "";
  size_t nextWSorNum = 0;

  stringstream ss;
  ss << input.substr(begin, end - begin) + ' ';

  ss >> label;

  nextWSorNum = nextNonWhitespaceOrNumber(input, begin);

  line = input.substr(nextWSorNum, end - nextWSorNum);

  return ALine(label, line);
}

const ALine ACode::getLine(const size_t label) const {
  vector<ALine>::const_iterator it = lines.begin();
  for (; it != lines.end(); it++) {
    if (it->label == label) {
      return (*it);
    }
  }
  return ALine(0, "");
}

//Returns the pos of the next non-whitespace char after the label
size_t ACode::nextNonWhitespaceOrNumber(const string &input, const size_t begin) const {
  bool foundNum = false;
  bool foundWsAfterNum = false;

  for (size_t i = begin; i < input.length(); i++) {

    if (input[i] == ' ') {
      foundWsAfterNum = true;
    } else if (isNumber(input[i])) {
      foundNum = true;
    }

    if (foundWsAfterNum && input[i] != ' ')
      return i;
  }
  return string::npos;
}

//Returns true if the inputted range of a string "stuff(thisIsAVar)stuff" is valid
bool ACode::isValidIdentifier(const string &iden, 
  const size_t start, const size_t end) const {
  //Reserved words
  if (iden.find("var ", start) <= end) {
    return false;
  } else if (iden.find("goto ", start) <= end) {
    return false;
  } else if (iden.find("print ", start) <= end) {
    return false;
  } else if (iden.find("stop ", start) <= end) {
    return false;
  } else if (iden.find("if ", start) <= end) {
    return false;
  }

  for (size_t i = start; i <= end; i++) {
    if (((iden[i]<'A' || iden[i]>'Z') && (iden[i]<'a' || iden[i]>'z'))) {
      if (i == start)
        return false;
      else if (iden[i]<'0' || iden[i]>'9')
        return false;
    }
  }
  return true;
}

int ACode::evalPostFix(const string &expr, const size_t label) const {
  std::stack<int> stk;
  stringstream strm;
  string toAdd = "";
  int converted = 0;

  //Stores currently operated on operands
  int firstInt = 0;
  int secondInt = 0;

  for (size_t i = 0; i < expr.size(); i++) {

    if (isNumber(expr[i]) || expr[i] == '!') {
      if (expr[i] == '!' && toAdd == "") {
        toAdd += '-';
      } else if (isNumber(expr[i])) {
        toAdd += expr[i];
      }
    } else if (toAdd != "") {
      strm << toAdd + " ";
      toAdd = "";
      converted = 0;
      strm >> converted;
      stk.push(converted);
    }

    if (isOperator(expr[i])) {
      if (stk.size() >= 2) {
        secondInt = stk.top();
        stk.pop();
        firstInt = stk.top();
        stk.pop();
        if (expr[i] == '/' && secondInt == 0) {
          cout << "LINE " << label << ": ";
          cout << "Cannot divide by 0!" << endl;
          exit(2);
        } else {
          stk.push(doOp(expr[i], firstInt, secondInt, label));
        }
      } else {
        cout << "LINE " << label << ": ";
        cout << "Expression must have at least 2 terms before an operator!" << endl;
        exit(2);
      }
    }
  }
  if (stk.size() != 1) {
    cout << "LINE " << label << ": ";
    cout << "Error processing expression!" << endl;
    exit(2);
  }
  return stk.top();
}

bool ACode::doesLineExist(const size_t label) const {
  vector<ALine>::const_iterator it = lines.begin();
  for (; it != lines.end(); it++) {
    if (it->label == label) {
      return true;
    }
  }
  return false;
}

bool ACode::delLineAt(const size_t label) {
  vector<ALine>::const_iterator it = lines.begin();
  for (; it != lines.end(); it++) {
    if (it->label==label) {
      lines.erase(it);
      return true;
    }
  }
  return false;
}

void ACode::toFile(const string & file) const {
  ofstream ostrm(file);
  vector<ALine>::const_iterator it = lines.begin();
  for (; it != lines.end(); it++) {
    ostrm << it->label << ' ' << it->line << ';' << endl;
  }
  ostrm.close();
}

void ACode::scanLines() const {
  vector<ALine>::const_iterator it = lines.begin();
  bool errorDetected = false;
  for (; it != lines.end(); it++) {
    if (!validateLine(*it)) {
      errorDetected = true;
    }
  }
  it--;
  if (it->line != "stop") {
    cout << "Last line must be a stop statement" << endl;
    errorDetected = true;
  }
  if (errorDetected) {
    cout << endl << "Syntax Errors detected, refer to above" << endl;
    exit(2);
  }
}

int ACode::doOp(const char op, const int first, const int second, const size_t label) const {
  switch (op) {
  case '+':
    return first + second;
    break;
  case '-':
    return first - second;
    break;
  case '*':
    return first * second;
    break;
  case '/':
    return first / second;
    break;
  default:
    cout << "LINE " << label << ": ";
    cout << "Operator not valid" << endl;
    exit(2);
    break;
  }
  return 0;
}

bool ACode::isLetter(const char input) const {
  return ((input >= 'a' && input <= 'z') || (input >= 'A' && input <= 'Z'));
}

bool ACode::isNumber(const char input) const {
  return (input >= '0' && input <= '9');
}

bool ACode::isAlphanumeric(const char input) const {
  return (isLetter(input) || isNumber(input));
}

//Converts infix(3+3) into postfix(3 3+)
string ACode::infixToPostfix(const string &infix) const {
  string postfix = "";
  std::stack<char> opStack;
  opStack.push('#');
  bool minusIsNegative = true; //Want to know if the next '-' we see is actually
                                //  a negative number
  char popOp;
  for (size_t i = 0; i < infix.size(); i++) {
    if (infix[i] != '#') {
      if (isNumber(infix[i])) {
        postfix += infix[i++];
        for (size_t j = i; j < infix.size(); j++) {
          i = j;
          if (isNumber(infix[j])) {
            postfix += infix[i];

          } else {
            postfix += ' ';
            minusIsNegative = false;
            break;
          }
        }
      }
      if (infix[i] == '(') {
        minusIsNegative = true; //Right after an opening parenthesis,
                                //  there can be a negative number
        opStack.push(infix[i]);
      }
      if (infix[i] == ')') {
        minusIsNegative = false;
        popOp = opStack.top();
        opStack.pop();
        while (popOp != '(') {
          postfix += popOp;
          postfix += ' ';
          popOp = opStack.top();
          opStack.pop();
        }
      }
      if (isOperator(infix[i])) {
        if (infix[i] == '-' && minusIsNegative) {
          postfix += '!';
          minusIsNegative = false;
        } else {
          minusIsNegative = true;
          while (getOperatorPrecedance(opStack.top())>=getOperatorPrecedance(infix[i])) {
            popOp = opStack.top();
            opStack.pop();
            postfix += popOp;
            postfix += ' ';
          }
          opStack.push(infix[i]);
        }
      }
    }
  }
  while (opStack.top() != '#') {
    popOp = opStack.top();
    opStack.pop();
    postfix += ' ';
    postfix += popOp;
  }
  return postfix;
}

STATEMENT ACode::getStatementType(const string &line) const {
  if (line.find("var ", 0) == 0) {
    return VAR;
  } else if ((line.find("var ", 0) == string::npos) && (line.find('=', 0) != string::npos)) {
    return ASSIGNMENT;
  } else if ((line.find("goto ", 0) != string::npos) && (line.find("if ", 0) == 0)) {
    return IF;
  } else if (line.find("print ", 0) == 0) {
    return PRINT;
  } else if (line.find("stop", 0) == 0) {
    return STOP;
  }
  return UNKNOWN;
}

//Returns whether or not the variable exists in the vars vector
bool ACode::doesVarExist(const string & var) const {
  vector<AVar>::const_iterator it = vars.begin();
  string internalVar = var.substr(0, var.find(' ', 0) - 1);
  for (; it != vars.end(); it++) {
    if (it->iden == internalVar) {
      return true;
    }
  }
  return false;
}

//Higher return == higher precendance
int ACode::getOperatorPrecedance(const char input) const {
  if (input == '+' || input == '-')
    return 0;
  if (input == '*' || input == '/')
    return 1;
  if (input == '^')
    return 2;
  return -1;
}

bool ACode::isOperator(const char input) const {
  return getOperatorPrecedance(input) != -1;
}

const size_t ACode::handleLine(const ALine &line) {
  size_t nextLine = line.label + 1;
  switch (getStatementType(line.line)) {
  case VAR:
    doVarStatement(line);
    break;
  case ASSIGNMENT:
    doAssignStatement(line);
    break;
  case IF:
    nextLine = doIfStatement(line);
    break;
  case PRINT:
    doPrintStatement(line);
    break;
  case STOP:
    doStopStatement(line);
    break;
  case UNKNOWN:
    cout << "LINE " << line.label << ": ";
    cout << "Unknown statement" << endl << endl;
    exit(2);
    break;
  }
  return nextLine;
}

bool ACode::isVarStatementValid(const ALine & line) const {
  size_t pos = line.line.find("var ", 0);
  string tempVarIden = "";
  bool foundNum = false;
  size_t i = pos + 4; //4 = size of "var "

  if (pos != string::npos) {
    while (line.line[i] == ' ' && i < line.line.length()) {
      i++;
    }
    if (isLetter(line.line[i])) {
      tempVarIden += line.line[i];
      i++;
      while (isAlphanumeric(line.line[i]) && i < line.line.length()) {
        tempVarIden += line.line[i];
        i++;
      }
      if (!isValidIdentifier(tempVarIden, 0, tempVarIden.length()-1)) {
        cout << "LINE " << line.label << ": ";
        cout << "Variable name: " << tempVarIden << " not allowed" << endl;
      }
      pos = line.line.find('=', i);
      if (pos == string::npos) {
        while (i < line.line.length()) {
          if (line.line[i] != ' ')
            return false;
          i++;
        }
        return true;
      } else {
        i = pos + 1;
        while (i < line.line.length()) {
          if (isLetter(line.line[i])) {
            cout << "LINE " << line.label << ": ";
            cout << "VAR Statement may only assign a number to a variable" << endl;
            return false;
          }
          if (!isNumber(line.line[i]) && foundNum) {
            cout << "LINE " << line.label << ": ";
            cout << "VAR Statement Invalid" << endl;
            return false;
          } else if (isNumber(line.line[i])) {
            foundNum = true;;
          }
          i++;
        }
        return true;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}

bool ACode::isAssignStatementValid(const ALine & line) const {
  size_t pos = line.line.find('=', 0) + 1;
  for (; pos < line.line.size(); pos++) {
    if (line.line[pos] != ' ') {
      return true;
    }
  }
  return false;
}

bool ACode::isIfStatementValid(const ALine & line) const {
  size_t firstOpenParen = line.line.find('(', 0);
  size_t lastOpenParen = 0;
  size_t gotoPos = line.line.find("goto ", 0);
  string labelString = "";
  size_t label = 0;
  stringstream strm;

  if (gotoPos == string::npos || firstOpenParen == string::npos) {
    return false;
  }

  for (size_t i = line.line.find("if ", 0) + 2; i < firstOpenParen; i++) {
    if (line.line[i] != ' ') {
      return false;
    }
  }
  for (size_t i = gotoPos - 1; i > firstOpenParen; i--) {
    if (line.line[i] == ')') {
      lastOpenParen = i;
      break;
    } else if (line.line[i] != ' ') {
      return false;
    }
  }
  for (size_t i = gotoPos + 5; i < line.line.length(); i++) {
    if (isNumber(line.line[i])) {
      labelString += line.line[i];
    } else if (line.line[i] != ' ') {
      return false;
    }
  }
  strm << labelString;
  strm >> label;
  ALine tempLine = getLine(label);
  return ((tempLine.line != "") && (tempLine.label != 0));
}

bool ACode::isPrintStatementValid(const ALine & line) const {
  for (size_t i = 6; i < line.line.length(); i++) {
    if (isAlphanumeric(line.line[i])) {
      return true;
    }
  }
  return false;
}

size_t ACode::doIfStatement(const ALine & line) const {
  string varIden = "";
  string expression = "";
  size_t openParenPos = line.line.find('(', 0);
  size_t closeParenPos = 0;
  size_t gotoPos = line.line.find("goto ", 0);
  size_t gotoLabel = 0;
  string labelString = "";

  stringstream strm;

  for (size_t i = gotoPos; i > openParenPos; i--) {
    if (line.line[i] == ')') {
      closeParenPos = i;
      break;
    }
  }
  for (size_t i = gotoPos + 5; i < line.line.length(); i++) {
    if (isNumber(line.line[i])) {
      labelString += line.line[i];
    }
  }
  strm << labelString;
  strm >> gotoLabel;
  openParenPos++;

  if (evalPostFix(infixToPostfix(resolveIdensInExpression(line.line,
    openParenPos, closeParenPos, line.label)) + ' ', line.label) != 0) {
    return gotoLabel;
  }
  return line.label + 1;
}

void ACode::doPrintStatement(const ALine & line) const {
  //print <expression>
  size_t i = string("print ").length();
  cout << evalPostFix(infixToPostfix(resolveIdensInExpression(line.line, i,
    line.line.length(), line.label)) + ' ', line.label) << endl;
}

void ACode::doStopStatement(const ALine & line) const {
  exit(0);
}

const ALine & ACode::getLineOrAfter(const size_t label) const {
  vector<ALine>::const_iterator it = lines.begin();
  for (; it != lines.end(); it++) {
    if (it->label >= label) {
      return *(it);
    }
  }
  cout << "LINE " << label << ": ";
  cout << "No line at or after " << label << endl << endl;
  exit(2);
  return *it;
}

void ACode::doVarStatement(const ALine & line) {
  string varIden = "";
  string expression = "";
  bool exprFound = false;
  size_t equalPos = line.line.find('=', 4);

  for (size_t i = 4; i < line.line.length(); i++) {
    if (isAlphanumeric(line.line[i])) {
      varIden += line.line[i];
    } else {
      break;
    }
  }

  if (equalPos != string::npos) {
    equalPos++;
    modifyVar(AVar(varIden, evalPostFix(infixToPostfix(resolveIdensInExpression(line.line, 
      equalPos, line.line.size(), line.label)) + ' ', line.label), true), line.label, false);
  } else {
    modifyVar(AVar(varIden, 0, false), line.label, false);
  }
}

void ACode::doAssignStatement(const ALine & line) {
  string varIden = "";
  string expression = "";
  bool found = false;
  size_t equalPos = line.line.find('=', 0);
  for (size_t i = 0; i < equalPos; i++) {
    if (found) {
      if (line.line[i] == ' ' || line.line[i] == '=') {
        break;
      } else {
        varIden += line.line[i];
      }
    } else if (isLetter(line.line[i])) {
      varIden += line.line[i];
      found = true;
    }
  }
  equalPos++;
  modifyVar(AVar(varIden, evalPostFix(infixToPostfix(resolveIdensInExpression(line.line, 
    equalPos, line.line.size(), line.label)) + ' ', line.label), true), line.label, true);
}

void ACode::executeCode() {
  ALine line = firstLine();
  scanLines();
  while (true) {
    line = getLineOrAfter(handleLine(line));
  }
}

bool ACode::validateLine(const ALine &line) const {
  switch (getStatementType(line.line)) {
  case VAR:
    if (!isVarStatementValid(line)) {
      return false;
    }
    return true;
    break;
  case ASSIGNMENT:
    if (!isAssignStatementValid(line)) {
      cout << "LINE " << line.label << ": ";
      cout << "Invalid ASSIGNMENT Statement" << endl;
      return false;
    }
    return true;
    break;
  case IF:
    if (!isIfStatementValid(line)) {
      cout << "LINE " << line.label << ": ";
      cout << "IF Statement Invalid" << endl;
      return false;
    }
    return true;
    break;
  case STOP:
    if (!(line.line.find("stop", 0) == 0 && line.line.length() == 4)) {
      cout << "LINE " << line.label << ": ";
      cout << "Invalid STOP Statement" << endl;
      return false;
    }
    return true;
    break;
  case PRINT:
    if (!isPrintStatementValid(line)) {
      cout << "LINE " << line.label << ": ";
      cout << "Invalid PRINT Statement" << endl;
      return false;
    }
    return true;
    break;
  case UNKNOWN:
    cout << "LINE " << line.label << ": ";
    cout << "Invalid Statement, statement type unknown" << endl;
    return false;
    break;
  }
  return false;
}

//EXPRESSION
//<char> <expression> | <var> <expression>
string ACode::resolveIdensInExpression(const string &expr, size_t &start,
  const size_t &end, const size_t &label) const {
  string toReturn = "";
  if (start == end) {
    return "";
  } else if (isLetter(expr[start])) {
    for (size_t i = start; i <= end; i++) {
      if ((i == end) || !isAlphanumeric(expr[i])) {
        string temp = "";
        stringstream strm;
        strm << convertIdenToVal(expr.substr(start, i - start), label);
        strm >> temp;
        toReturn = temp;
        start = i - 1;
        break;
      }
    }
  } else {
    toReturn = expr[start];
  }
  start++;
  return toReturn + resolveIdensInExpression(expr, start, end, label);
}

//Finds the corresponding variable in the vars vector and returns the value
int ACode::convertIdenToVal(const string &var, const size_t label) const {
  vector<AVar>::const_iterator it = vars.begin();
  for (; it != vars.end(); it++) {
    if (it->iden == var && it->initialized) {
      return it->val;
    }
  }
  cout << "LINE " << label << ": ";
  cout << "Variable " << '\'' << var << '\'' << " uninitialized." << endl << endl;
  exit(2);
}
