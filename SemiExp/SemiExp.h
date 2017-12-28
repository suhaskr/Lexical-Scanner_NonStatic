#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Suhas Kamasetty Ramesh, Syracuse University          //
//              skamaset@syr.edu                                     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a 
* sequence of tokens that ends in {.  This will be extended to use the
* full set of terminators: {, }, ;, and '\n' if the line begins with #.
*
* This is a new version for students in CSE687 - OOD, Spring 2016 to use
* for Project #1.
*
*
*  Public Interface:
*  ---------------------------------
*
* get(bool clear=true) - add tokens until one of the termination condition is met. 
*                        (clear the vector before adding new tokens if clear === true)
*
* length() - return the number of tokens present in the semiexpression.
*
* std::string& operator[](int n) - returns the token at index n
*
* find(const std::string& tok) - function to find the position of token - tok
*
* remove(const std::string& tok) - function to remove the token(tok). (First occurence of it is removed)
*
* remove(size_t n) - function to remove the token at index n
*
* push_back(const std::string& tok) - function to add the token tok to the existing vector of tokens.
*
* toLower() - Convert all the upper case leeters in the semi expression to lower case.
*
* trimFront() - Remove all leading newline characters in the semi expression.
*
* clear() - Remove all the existing tokens in the semi expression
*
* removePairsOfParantheses(std::vector<std::string>& vect) - Removes pairs of paranthese () in the semi expression. 
*                                                             This is used to find if semicolon is present inside the foor loop.
*
* isSemiWithinParantheses() - Check if semicolon is present within the parantheses tokens of for loop. 
*
* show(bool showNewLines=false) - Display all tokens in the semi expression space separated.
*                                (If showNewLines == True, newline characters as displayed as "newLine" literals else they are not displayed)
*
* Build Process:
* --------------
* Required Files: 
*   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
* 
* Build Command: devenv Project1.sln /rebuild debug
*
*
* Maintenance History:
* --------------------
* ver 3.3 : 07 Feb 2016
* - Implemented the interface ITokCollection
* - added public :, protected :, private : as terminators
* - provided a rule to omit termination of collection of tokens
*   in certain cases like 
* ver 3.2 : 02 Feb 2016
* - declared SemiExp copy constructor and assignment operator = delete
* - added default argument for Toker pointer to nullptr so SemiExp
*   can be used like a container of tokens.
* - if pToker is nullptr then get() will throw logic_error exception
* ver 3.1 : 30 Jan 2016
* - changed namespace to Scanner
* - fixed bug in termination due to continually trying to read
*   past end of stream if last tokens didn't have a semiExp termination
*   character
* ver 3.0 : 29 Jan 2016
* - built in help session, Friday afternoon
*
* Planned Additions and Changes:
* ------------------------------
* - move creation of tokenizer into semiExp constructor so
*   client doesn't have to write that code.
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "Itokcollection.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp : public ITokCollection
  {
  public:
    SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
    SemiExp& operator=(const SemiExp&) = delete;
    bool getHelper(bool clear=true);
    //Token operator[](size_t n);
	bool isSemiWithinParantheses();
	//bool isPreprocessorDirective();
	void removePairsOfParantheses(std::vector<std::string>& vect);
	int indexOf(const std::string& tok, const std::vector<std::string>& vect);

	//Implement pure virtual functions of Itokcollection.h
	bool get(bool clear = true);
	std::string& operator[](int n);
	size_t length();
	size_t find(const std::string& tok);
	size_t find_last(const std::string& tok);
	void push_back(const std::string& tok);
	bool remove(const std::string& tok);
	bool remove(size_t i);
	void toLower();
	void trimFront();
	void clear();
	std::string show(bool showNewLines = false);

  private:
    std::vector<Token> _tokens;
    Toker* _pToker;
	bool hasFor = false;
  };
}
#endif
