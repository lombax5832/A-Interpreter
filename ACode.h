#pragma once
using namespace std;

#include<vector>

//Constant Statement types
const enum STATEMENT {VAR, ASSIGNMENT, IF, PRINT, STOP, UNKNOWN};
const string Statements[] = { "VAR" , "ASSIGNMENT", "IF", "PRINT", "STOP", "UNKNOWN" };
const int STATEMENT_VAR = 0;
const int STATEMENT_ASSIGNMENT = 1;
const int STATEMENT_IF = 2;
const int STATEMENT_PRINT = 3;
const int STATEMENT_STOP = 4;

struct ALine {
  size_t label;
  string line;
  ALine(size_t lbl, string &ln) {
    label = lbl;
    line = ln;
  }
};

struct AVar {
  string iden;
  int val;
  AVar(string id, int in) {
    iden = id;
    val = in;
  }
};

class ACode{
public:
  ACode();
  ACode(const string&);

  void fromText(const string&);

  void printLines() const;
  
  STATEMENT getStatementType(const string &line) const;
private:
  vector<ALine> lines;

  vector<AVar> vars;

  void addline(ALine);
  
  ALine textToLine(const string&, const size_t begin, const size_t end) const;

  size_t nextNonWhitespaceOrNumber(const string&, const size_t begin) const;

  bool isValidIdentifier(const string &iden, const size_t, const size_t end) const;

  

  bool doesVarExist(const string& var) const;
};