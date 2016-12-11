#pragma once
using namespace std;

#include<vector>

//Constant Statement types
const enum STATEMENT {VAR, ASSIGNMENT, IF, PRINT, STOP, UNKNOWN};
const enum  SYNTAX_ERR {NONE, UNDEC_VAR};

//Line of code, splits up line and label
struct ALine {
  size_t label;
  string line;
  ALine(size_t lbl, string ln) {
    label = lbl;
    line = ln;
  }
};

//Variable, splits up identifier, value, and whether or not is initialized
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
  // Default constructor
  ACode();

  // Returns reference to first line for entry point during execution
  const ALine& firstLine() const;

  /*
  PRE: input string has lines followed by semi-colon characters
    ex: <line 1>;<line 2>;
  POST: lines will be stored in lines vector of ALine objects
  Parameters:
    lines: continuous string of A++ lines separated by semi-colons
  */
  void fromText(const string &lines);

  /*
  PRE: input file has lines on different lines of the file, with semi-colons
    terminating them
    ex:
      <line 1>;
      <line 2>;
  POST: file will be read and each line will be converted into an ALine object
    and inserted into lines vector of ALine objects
  Parameters:
    file: name/location of file to read
    strict: whether or not a bool should be returned, or if the program should exit
      if an error is encountered
  */
  bool fromFile(const string &file, bool strict);

  /*
  PRE: file with name of fromKeyboard.txt will be searched for
  POST: if file is found, its lines will be used initially
  */
  void fromKeyboard();

  /*
  PRE: at least 1 line is in lines vector
  POST: prints all lines in order with labels
  */
  void printLines() const;  

  /*
  PRE: at least 1 line is in lines vector
  POST: reads each line to make sure that all goto statements
    go to a line, all expressions have declared variables,
    all assignment statements have a declared varaible, etc.
  */
  void scanLines() const;

  /*
  PRE: at least 1 line is in lines vector
  POST: scanLines() is executed and code is then executed
  */
  void executeCode();
private:
  //Stores lines of code
  vector<ALine> lines;
  //Stores vars
  vector<AVar> vars;

  /*
  PRE: line is a valid ALine
  POST: line will be added to lines vector
  Parameters:
    line: ALine object with label and line
    strict: true: return error, else return false
  */
  void addline(const ALine line, bool strict);

  /*
  PRE: var is a valid AVar
  POST: var will be added to vars vector
  Parameters:
    var: AVar object with identifier and value
    label: line label to trace back to if an error occurs
  */
  void addVar(const AVar var, const size_t label);

  /*
  PRE: var is a valid AVar
  POST: var will be added to vars vector if it doesnt exist
    or will change var variable with the same identifier
  Parameters:
    var: AVar object with identifier and value
    label: line label to trace back to if an error occurs
    strict: strict: true: return error, else return false
  */
  void modifyVar(const AVar var, const size_t label, bool strict);
  
  /*
  PRE: string is a valid line with a label and ending semi-colon
  POST: string will be converted into an ALine object
  Parameters:
    line: contains label at beginning and semi-colon at end
    begin: first char position of line, usually 0;
    end: last char position of line, usually line.length()
  */
  ALine textToLine(const string& line, const size_t begin, const size_t end) const;

  /*
  PRE: label is an initialized size_t
  POST: ALine object corresponding with the label will be returned
  Parameters:
    label: label of a line object
  */
  const ALine getLine(const size_t label) const;

  /*
  PRE: begin < str.length()
  POST: returns position of next Letter
  Parameters:
    str: string to look within
    begin: position to begin search at
  */
  size_t nextNonWhitespaceOrNumber(const string &str, const size_t begin) const;

  /*
  PRE: start < end
  POST: returns whether or not start <= iden <= end is a valid identifier
  Parameters:
    iden: string containing segment to check for a valid identifier in
    start: identifier start position
    end: identifier end position
  */
  bool isValidIdentifier(const string &iden, const size_t start, const size_t end) const;

  /*
  PRE: op is a valid mathematical operator
  POST: result of operation will be returned, if dividing by zero, program will
    print an error and terminate
  Parameters:
    op: valid mathematical operator: +,-,*,/
    first: first operand
    second: second operand
    label: line label to trace error to
  */
  int doOp(const char op, const int first, const int second, const size_t label) const;

  /*
  PRE: ch is an initialized char
  POST: returns true if ch corresponds with a letter
  Parameters:
    ch: char to test
  */
  bool isLetter(const char ch) const;

  /*
  PRE: ch is an initialized char
  POST: returns true if ch corresponds with a number
  Parameters:
    ch: char to test
  */
  bool isNumber(const char ch) const;

  /*
  PRE: ch is an initialized char
  POST: returns true if ch corresponds with a letter or number
  Parameters:
    ch: char to test
  */
  bool isAlphanumeric(const char ch) const;

  /*
  PRE: var is a valid identifier name
  POST: returns true if the variable exists in the vars vector
  Parameters:
    var: identifier name to test
  */
  bool doesVarExist(const string& var) const;

  /*
  PRE: op is a valid operator
  POST: returns the precedance of given operator,
    higher: greater precedance
  Parameters:
    op: operator to test
  */
  int getOperatorPrecedance(const char op) const;

  /*
  PRE: op is initialized char
  POST: returns true if ch corresponds with an operator
  Parameters:
    ch: char to test
  */
  bool isOperator(const char ch) const;

  /*
  PRE: &line is a valid ALine object
  POST: returns the next line to be executed
  Parameters:
    line: line object from lines vector
  */
  const size_t handleLine(const ALine &line);

  /*
  PRE: line is an initialized ALine object
  POST: returns whether or not the line is valid
  Parameters:
    line: ALine to test
  */
  bool validateLine(const ALine &line) const;

  /*
  PRE: line is an initialized ALine object
  POST: returns whether or not the line is a valid statement
    of the type
  Parameters:
    line: ALine to test
  */
  bool isVarStatementValid(const ALine &line) const;
  bool isAssignStatementValid(const ALine &line) const;
  bool isIfStatementValid(const ALine &line) const;
  bool isPrintStatementValid(const ALine &line) const;

  /*
  PRE: line is an initialized ALine object
  POST: executes if statement, and returns the next line
  Parameters:
    line: ALine to test
  */
  size_t doIfStatement(const ALine &line) const;

  /*
  PRE: line is an initialized ALine object
  POST: executes statement
  Parameters:
    line: ALine to test
  */
  void doPrintStatement(const ALine &line) const;
  void doStopStatement(const ALine &line) const;
  void doVarStatement(const ALine &line);
  void doAssignStatement(const ALine &line);

  /*
  PRE: label is an initialized size_t
  POST: returns the line with the label or after the label passed
  Parameters:
    label: label of ALine to look for
  */
  const ALine& getLineOrAfter(const size_t label) const;

  /*
  PRE: line is an initialized ALine object
  POST: returns the type of statement that line corresponds to
  Parameters:
    line: ALine to test
  */
  STATEMENT getStatementType(const string &line) const;

  /*
  PRE: returns stored value for variable corresponding to var
  POST: executes if statement, and returns the next line
  Parameters:
    var: variable identifier to look for
    label: label to trace error to
  */
  int convertIdenToVal(const string &var, const size_t label) const;

  /*
  PRE: infix is a string representing an expression in infix notation
  POST: postfix expression is returned,
    error may result if infix expression is incorrect
  Parameters:
    infix: expression to convert to postfix
  */
  string infixToPostfix(const string &infix) const;

  /*
  PRE: expr is a line with an expression containing numbers,
    operators, or identifiers
  POST: returns a string with all identifiers
    replaced with their stored value
  Parameters:
    expr: string containing expression
    start: first char pos of expression
    end: last char pos of expression
    label: label to trace error to
  */
  string resolveIdensInExpression(const string &expr, size_t &start, 
    const size_t &end, const size_t &label) const;

  /*
  PRE: expr is a postfix expression
  POST: integer value of evaluated expression is returned
  Parameters:
    expr: postfix expression
    label: label to trace error to
  */
  int evalPostFix(const string &expr, const size_t label) const;

  /*
  PRE: label is an initialized size_t
  POST: returns whether a line corresponding with label exists
  Parameters:
    label: label to test
  */
  bool doesLineExist(const size_t label) const;

  /*
  PRE: label is an initialized size_t
  POST: returns whether the deletion operation was successful
  Parameters:
    label: label of line to delete
  */
  bool delLineAt(const size_t label);

  /*
  PRE: file is a valid filename
  POST: saves lines of App code to a file
  Parameters:
    file: file name to save lines to
  */
  void toFile(const string &file) const;
};