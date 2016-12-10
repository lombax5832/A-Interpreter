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
void ACode::addline(const ALine line) {
  //Preserves order to use binary search later
  if (lines.empty()) {
    lines.push_back(line);
    return;
  }

  if (line.label > lines.back().label) {
    lines.push_back(line);
  } else {
    cout << "LINE " << line.label << ": ";
    cout << "ERROR, added label must be larger than previous labels." << endl;
    exit(2);
  }
}

void ACode::addVar(const AVar toAdd, const size_t label) {
  if (!doesVarExist(toAdd.iden)) {
    vars.push_back(toAdd);
  } else {
    cout << "LINE " << label << ": ";
    cout << "Var " << toAdd.iden << " already exists." << endl;
    exit(2);
  }
}

void ACode::modifyVar(const AVar toModify, const size_t label) {
  vector<AVar>::iterator it = vars.begin();
  for (; it != vars.end(); it++) {
    if (it->iden == toModify.iden) {
      it->val = toModify.val;
      break;
    }
  }
  cout << "LINE " << label << ": ";
  cout << "Var " << toModify.iden << " does not exist." << endl;
  exit(2);
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

    if (input[i] == ' ') {
      foundWsAfterNum = true;
    } else if (isNumber(input[i])) {
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
      converted=0;
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
    cout << "More than 1 item in stack after expression is handled!" << endl;
    exit(2);
  }
  return stk.top();
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
          while (getOperatorPrecedance(opStack.top()) >= getOperatorPrecedance(infix[i])) {
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

void ACode::handleLine(const ALine line) {
  switch (getStatementType(line.line)) {
  case VAR:
    break;
  case ASSIGNMENT:
    break;
  case IF:
    break;
  case STOP:
    break;
  case PRINT:
    break;
  case UNKNOWN:
    break;
  }
}

//EXPRESSION
//<char> <expression> | <var> <expression>
string ACode::resolveIdensInExpression(const string &expr, size_t start, const size_t end) const {
  string toReturn = "";
  if (start == end) {
    return "";
  }
  else if (isLetter(expr[start])) {
    for (size_t i = start; i <= end; i++) {
      if ((i == end) || !isAlphanumeric(expr[i])) {
        string temp = "";
        stringstream strm;
        strm << convertIdenToVal(expr.substr(start, i - start));
        strm >> temp;
        toReturn = temp;
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
