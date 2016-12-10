#pragma once
using namespace std;

#include<vector>

//Constant Statement types
const enum STATEMENT {VAR, ASSIGNMENT, IF, PRINT, STOP, UNKNOWN};
//const string STATEMENT[] = { "VAR" , "ASSIGNMENT", "IF", "PRINT", "STOP", "UNKNOWN" };
const enum  SYNTAX_ERR {NONE, UNDEC_VAR};

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

  int convertIdenToVal(const string &var) const;

  string infixToPostfix(const string &infix) const;

  string resolveIdensInExpression(const string &expr) const;
private:
  vector<ALine> lines;

  vector<AVar> vars;

  void addline(ALine);
  
  ALine textToLine(const string&, const size_t begin, const size_t end) const;

  size_t nextNonWhitespaceOrNumber(const string&, const size_t begin) const;

  bool isValidIdentifier(const string &iden, const size_t, const size_t end) const;

  int handleExpression(const string &expr) const;

  

  bool doesVarExist(const string& var) const;

  int getOperatorPrecedance(const char input) const;

  bool isOperator(const char input) const;

};