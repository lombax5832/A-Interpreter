#include "ACode.h"

using namespace std;

#include<iostream>
#include<sstream>
#include<cmath>

ACode::ACode() {
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

bool ACode::isValidIdentifier(const string &iden) const {
  for (int i = 0; i < iden.size(); i++) {
    if (((iden[i]<'A' || iden[i]>'Z') && (iden[i]<'a' || iden[i]>'z'))) {
      if (i == 0)
        return false;
      else if (iden[i]<'0' || iden[i]>'9')
        return false;
    }
  }
  return true;
}
