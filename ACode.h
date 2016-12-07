#pragma once
using namespace std;

#include<vector>

struct ALine {
  size_t label;
  string line;
  ALine(size_t lbl, string &ln) {
    label = lbl;
    line = ln;
  }
};

class ACode{
public:
  ACode();
  ACode(const string&);

  void fromText(const string&);

  void printLines() const;
private:
  vector<ALine> lines;

  void addline(ALine);
  
  
  ALine textToLine(const string&) const;
  ALine textToLine(const string&, const size_t begin, const size_t end) const;

  size_t nextNonWhitespaceOrNumber(const string&, const size_t begin) const;
};