#include "ACode.h"

using namespace std;

#include<iostream>
#include<sstream>
#include<cmath>

ACode::ACode() {
  vars.push_back(AVar("abc", 0));
}

ACode::ACode(const string &input) {
}

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

void ACode::printLines() const {
  if (!lines.empty()) {
    for (size_t i = 0; i < lines.size(); i++) {
      cout << lines.at(i).label << ' ' << lines.at(i).line << endl;
    }
  }
}

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
