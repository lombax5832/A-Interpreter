#include "ACode.h"

using namespace std;

#include<iostream>
#include<sstream>
#include<cmath>
#include<stack>

ACode::ACode() {
  //test var
  vars.push_back(AVar("abc", 20));
}

ACode::ACode(const string &input) {
}

//Adds a line to lines vector if label is greater than previous label
void ACode::addline(ALine line) {
  //Preserves order to use binary search later
  if (lines.empty()) {
    lines.push_back(line);
    return;
  }

  if (line.label > lines.back().label) {
    lines.push_back(line);
  } else {
    cout << "ERROR, added label must be larger than previous labels." << endl;
    exit(2);
  }
}

//Using a large string, stores lines in lines vector
void ACode::fromText(const string &input) {
  size_t pos = 0;
  size_t lastPos = 0;
  size_t label = 0;
  string line = "";

  pos = input.find(";", 0);

  while (pos != string::npos) {
    addline(textToLine(input, lastPos, pos));

    lastPos = pos + 1;
    pos = input.find(";", pos + 1);
  }
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
ALine ACode::textToLine(const string& input, const size_t begin, const size_t end) const {
  size_t label = 0;
  string line = "";
  size_t nextWSorNum = 0;

  stringstream ss;
  ss << input.substr(begin, end - begin);

  ss >> label;

  nextWSorNum = nextNonWhitespaceOrNumber(input, begin);

  line = input.substr(nextWSorNum, end - nextWSorNum);

  //cout << line << endl;

  return ALine(label, line);
}

//Returns the pos of the next non-whitespace char after the label
size_t ACode::nextNonWhitespaceOrNumber(const string &input, const size_t begin) const {
  bool foundNum = false;
  bool foundWsAfterNum = false;

  for (size_t i = begin; i < input.length(); i++) {

    if (input[i] == ' ' && (input[i] < '0' || input[i] > '9')) {
      foundWsAfterNum = true;
    } else if (input[i] >= '0' && input[i] <= '9') {
      foundNum = true;
    } 

    if(foundWsAfterNum && input[i] != ' ')
      return i;
  }
  return string::npos;
}

//Returns true if the inputted range of a string "stuff(thisIsAVar)stuff" is valid
bool ACode::isValidIdentifier(const string &iden, const size_t start, const size_t end) const {
  //Reserved words
  if (iden.find("var ", start) <= end) {
    return false;
  }
  else if (iden.find("goto ", start) <= end) {
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

//TODO
int ACode::handleExpression(const string &expr) const {
  return 0;
}

//Converts infix(3+3) into postfix(3 3+)
string ACode::infixToPostfix(const string &infix) const {
  string postfix = "";
  std::stack<char> stack;
  stack.push('#');
  bool minusIsNegative = false; //Want to know if the next '-' we see is actually
                                //  a negative number
  char popOp;
  for (size_t i = 0; i < infix.size(); i++) {
    if (infix[i] != '#') {
      if ((infix[i] >= '0' && infix[i] <= '9')) {
        postfix += infix[i++];
        for (size_t j = i; j < infix.size(); j++) {
          i = j;
          if (infix[j] >= '0' && infix[j] <= '9') {
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
        stack.push(infix[i]);
      }
      if (infix[i] == ')') {
        minusIsNegative = false;
        popOp = stack.top();
        stack.pop();
        while (popOp != '(') {
          postfix += popOp;
          postfix += ' ';
          popOp = stack.top();
          stack.pop();
        }
      }
      if (isOperator(infix[i])) {
        if (infix[i] == '-' && minusIsNegative) {
          postfix += '-';
          minusIsNegative = false;
        } else {
          minusIsNegative = true;
          while (getOperatorPrecedance(stack.top()) >= getOperatorPrecedance(infix[i])) {
            popOp = stack.top();
            stack.pop();
            postfix += popOp;
            postfix += ' ';
          }
          stack.push(infix[i]);
        }
      }
    }
  }
  while (stack.top() != '#') {
    popOp = stack.top();
    stack.pop();
    postfix += popOp;
  }
  return postfix;
}

STATEMENT ACode::getStatementType(const string &line) const {
  if (line.find("var ", 0) == 0) {
    return VAR;
  }
  else if (doesVarExist(line.substr(0, line.find('=', 0) - 1))) {
    return ASSIGNMENT;
  }
  else if ((line.find("goto ", 0) != string::npos) && (line.find("if ", 0) == 0)) {
    return IF;
  }
  else if (line.find("print ", 0) == 0) {
    return PRINT;
  }
  else if (line.find("stop", 0) == 0) {
    return STOP;
  }
  return UNKNOWN;
}

//Returns whether or not the variable exists in the vars vector
bool ACode::doesVarExist(const string & var) const {
  vector<AVar>::const_iterator it = vars.begin();
  //chop off trailing blank spaces
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

//EXPRESSION
//<char> <expression> | <var> <expression>
string ACode::resolveIdensInExpression(const string &expr, size_t start, const size_t end) const {
  string toReturn = "";
  if (start == end) {
    return "";
  }
  else if (((expr[start] >= 'a'&&expr[start] <= 'z') || (expr[start] >= 'A'&&expr[start] <= 'Z'))) {
    for (size_t i = start; i <= end; i++) {
      if ((i == end) || !((expr[i] >= 'a'&&expr[i] <= 'z') || (expr[i] >= 'A'&&expr[i] <= 'Z') || (expr[i] >= '0'&&expr[i] <= '9'))) {
        string temp = "";
        stringstream strm;
        strm << convertIdenToVal(expr.substr(start, i - start));
        strm >> temp;
        toReturn = temp;
        cout << toReturn << ';' << endl;
        start = i-1;
        break;
      }
    }
  } else {
    toReturn = expr[start];
  }
  start++;
  return toReturn + resolveIdensInExpression(expr, start, end);
}

//Finds the corresponding variable in the vars vector and returns the value
int ACode::convertIdenToVal(const string &var) const {
  vector<AVar>::const_iterator it = vars.begin();
  for (; it != vars.end(); it++) {
    if (it->iden == var) {
      return it->val;
    }
  }
  return 0;
}
