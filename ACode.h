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
  ALine(size_t lbl, string ln) {
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
  AVar* AVar::getThis() {
    return this;
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

  string resolveIdensInExpression(const string &expr, size_t start, const size_t end) const;

  int evalPostFix(const string &expr, const size_t label) const;

  bool isVarStatementValid(const ALine &line) const;
private:
  vector<ALine> lines;

  vector<AVar> vars;

  void addline(const ALine);

  void addVar(const AVar, const size_t label);

  void modifyVar(const AVar, const size_t label);
  
  ALine textToLine(const string&, const size_t begin, const size_t end) const;

  size_t nextNonWhitespaceOrNumber(const string&, const size_t begin) const;

  bool isValidIdentifier(const string &iden, const size_t start, const size_t end) const;

  int doOp(const char op, const int first, const int second, const size_t label) const;

  bool isLetter(const char) const;

  bool isNumber(const char) const;

  bool isAlphanumeric(const char) const;

  bool doesVarExist(const string& var) const;

  int getOperatorPrecedance(const char input) const;

  bool isOperator(const char input) const;

  void handleLine(const ALine &line);

  void validateLine(const ALine &line) const;

  
};