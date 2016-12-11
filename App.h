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
  bool initialized;
  AVar(string id, int in, bool decl) {
    iden = id;
    val = in;
    initialized = decl;
  }
  AVar* AVar::getThis() {
    return this;
  }
};

class ACode{
public:
  ACode();
  ACode(const string&);

  const ALine& firstLine() const;

  void fromText(const string&);

  void fromFile(const string &file);

  void printLines() const;  

  void scanLines() const;

  void executeCode();
private:
  vector<ALine> lines;

  vector<AVar> vars;

  void addline(const ALine);

  void addVar(const AVar, const size_t label);

  void modifyVar(const AVar, const size_t label, bool strict);
  
  ALine textToLine(const string&, const size_t begin, const size_t end) const;

  const ALine getLine(const size_t label) const;

  size_t nextNonWhitespaceOrNumber(const string&, const size_t begin) const;

  bool isValidIdentifier(const string &iden, const size_t start, const size_t end) const;

  int doOp(const char op, const int first, const int second, const size_t label) const;

  bool isLetter(const char) const;

  bool isNumber(const char) const;

  bool isAlphanumeric(const char) const;

  bool doesVarExist(const string& var) const;

  int getOperatorPrecedance(const char input) const;

  bool isOperator(const char input) const;

  const size_t handleLine(const ALine &line);

  bool validateLine(const ALine &line) const;

  bool isVarStatementValid(const ALine &line) const;

  bool isAssignStatementValid(const ALine &line) const;

  bool isIfStatementValid(const ALine &line) const;

  bool isPrintStatementValid(const ALine &line) const;

  size_t doIfStatement(const ALine &line) const;

  void doPrintStatement(const ALine &line) const;

  void doStopStatement(const ALine &line) const;

  void doVarStatement(const ALine &line);

  void doAssignStatement(const ALine &line);

  const ALine& getLineOrAfter(const size_t label) const;

  STATEMENT getStatementType(const string &line) const;

  int convertIdenToVal(const string &var, const size_t label) const;

  string infixToPostfix(const string &infix) const;

  string resolveIdensInExpression(const string &expr, size_t &start, const size_t &end, const size_t &label) const;

  int evalPostFix(const string &expr, const size_t label) const;
};