///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.3                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Suhas Kamasetty Ramesh, Syracuse University          //
//              skamaset@syr.edu                                     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

//-------< Constructor of SemiExp class>-------------------------
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

//-------< REturn token at index n>-------------------------
std::string& SemiExp::operator[](int n) {
	if (n < 0 || (size_t)n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

//-------< Return index of token - tok >-------------------------
size_t SemiExp::find(const std::string& tok) {
	for (size_t i = 0; i < _tokens.size();i++) {
		if (tok == _tokens[i])
			return i;
	}
	return _tokens.size(); //This is set to size() so that it will be out of bounds if some one uses it to acces element.
}

//-------< Return index of token - tok >-------------------------
// note: The difference here is that it also takes in a vector as an argument.
int SemiExp::indexOf(const std::string& tok, const std::vector<std::string>& vect) {
	for (size_t i = 0; i < vect.size(); i++) {
		if (tok == vect[i])
			return i;
	}
	return -1;
}

//-------< Return index of token - tok >-------------------------
//Note: The normal find returns first occurrence. This returns last occurrence.
size_t SemiExp::find_last(const std::string& tok) {
	for (int i = (_tokens.size() - 1); i >= 0; i--)
		if (tok == _tokens[i])
			return i;
	return 999;
}

//-------< Add the token tok to the existing vector of tokens >-------------------------
void SemiExp::push_back(const std::string& tok) {
	_tokens.push_back(tok);
}

//-------< Remove the token tok. >-------------------------
// Note: If token does not exist return false, else return true.
bool SemiExp::remove(const std::string& tok) {
	size_t index = find(tok);
	if (index < length()) {
		_tokens.erase(_tokens.begin() + index);
		return true;
	}
	else {
		return false;
	}
}

//-------< Remove the token at index i >-------------------------
//Note: If token is removed returns true else false.
bool SemiExp::remove(size_t i) {
	if (i < length()) {
		_tokens.erase(_tokens.begin() + i);
		return true;
	}
	else {
		return false;
	}
}

//-------< Convert all the upper case leeters in the semi expression to lower case >-------------------------
void SemiExp::toLower() {
	for (size_t i = 0; i < _tokens.size(); i++) {
		for (size_t j = 0; j < _tokens[i].length(); j++)
			_tokens[i][j] = tolower(_tokens[i][j]);
	}
}

//-------< Remove all leading newline characters in the semi expression. >-------------------------
void SemiExp::trimFront() {
	if (_tokens[0] == "\n") {
		_tokens.erase(_tokens.begin());
		trimFront();
	}
}

//-------< Remove all the existing tokens in the semi expression >-------------------------
void SemiExp::clear() {
	_tokens.erase(_tokens.begin(), _tokens.end());
}

//-------< Removes pairs of paranthese() in the semi expression >-------------------------
void SemiExp::removePairsOfParantheses(std::vector<std::string>& vect) {
	//int openBrace = indexOf("(", vect);
	//int closeBrace = indexOf(")", vect);
	if (indexOf("(", vect) != -1 && indexOf(")", vect) != -1) {
		vect.erase(vect.begin() + indexOf("(", vect));
		vect.erase(vect.begin() + indexOf(")", vect));
		removePairsOfParantheses(vect);
	}
}

//-------< Check if semicolon is present within the paranthese of for loop>-------------------------
bool SemiExp::isSemiWithinParantheses() {
	size_t openBracketPosition = find("(");
	size_t closeBracketPosition = find(")");
	size_t semiPosition = find(";");
	//std::cout << "open=" << openBracketPosition << " close=" << closeBracketPosition << " semi=" << semiPosition;
	if (semiPosition > openBracketPosition && semiPosition < closeBracketPosition)
		return true;
	else {
		std::vector<std::string> temp = _tokens;
		removePairsOfParantheses(temp);
		if (indexOf("(", temp) == -1)
			return false;
		else {
			return true;
		}
	}
}


//-------< Add tokens until one of the termination conditions for semiexpression is met >----------------
bool SemiExp::get(bool clear) {
	hasFor = false;
	bool ret_val = getHelper(clear);
	//std::cout << "value of has for = " << hasFor << std::endl;
	if (hasFor && isSemiWithinParantheses()) {
		ret_val = getHelper(false);
		ret_val = getHelper(false);
	}
	return ret_val;
}

//-------< Add tokens until one of the termination conditions for semiexpression is met >----------------
bool SemiExp::getHelper(bool clear)
{
	bool isPreprocessorDirective = false;
  if (_pToker == nullptr)
    throw(std::logic_error("no Toker reference"));

  if (clear)
	  _tokens.clear();

  while (true)
  {
    std::string token = _pToker->getTok();
    if (token == "")
      break;
    _tokens.push_back(token);
	
	//Handle pre processor directive
	if (token == "#") {
		trimFront();
		if (length() == 1)
			isPreprocessorDirective = true;
	}

	if (isPreprocessorDirective && token == "\n") {
		isPreprocessorDirective = false;
		return true;
	}

	if (token == "for")
		hasFor = true;
		
	if (token == ":") {
		std::string temp = _tokens[_tokens.size() - 2];
		if (temp == "public" || temp == "protected" || temp == "private")
			return true;
		
	}
	if (token == "{" || token == "}" || token == ";")
      return true;
  }
  return false;
}

/* Not used anymore as [] operator is implemented as in the ITokCollection interface
Token SemiExp::operator[](size_t n)
{
  if (n < 0 || n >= _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}*/

//-------< Returns the number of tokens in the semi expression >----------------
size_t SemiExp::length()
{
  return _tokens.size();
}


//-------< Display all the tokens in the semiexpression >----------------
std::string SemiExp::show(bool showNewLines)
{
  std::cout << "\n  ";
  //toLower();
  std::string temp = "";
  for (auto token : _tokens)
	  if (token != "\n") {
		  std::cout << token << " ";
		  temp += token;
		  temp += " ";
	  }	
	  else {
		  if (showNewLines) {
			  std::cout << "newline ";
			  temp += "newline ";
		  }			  
	  }
		  
  std::cout << "\n";
  return temp;
}

#ifdef TEST_SEMIEXP
int main()
{
  Toker toker;
  std::string fileSpec = "../TestFiles/SemiExpressionTest1.txt";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);
  toker.returnComments(false);
  SemiExp semi(&toker);
  while(semi.get())
  {
    std::cout << "\n  -- semiExpression --";
    semi.show();
  }
  /*
     May have collected tokens, but reached end of stream
     before finding SemiExp terminator.
   */
  if (semi.length() > 0)
  {
    std::cout << "\n  -- left out semiExpression --";
    semi.show();
    std::cout << "\n\n";
  }

  //Test the interface methods:
  Toker toker4;
  std::fstream in4("./../TestFiles/SemiExpressionTest2.txt");
  if (!in4.good()) {
	  std::cout << "could not open file: SemiExpressionTest2.txt";
	  return 1;
  }
  toker4.attach(&in4);
  SemiExp semi2(&toker4);

  std::cout << std::endl << "  -- semi.get(clear=true) --" << std::endl;
  std::cout << "      " << semi2.get(true);

  std::cout << std::endl << "  -- semi.show(showNewLines=false) -- ";
  semi2.show(false);

  std::cout << std::endl << "  -- semi.show(showNewlines=true) --";
  std::string returnOfShow = semi2.show(true);

  std::cout << "  -- semi.trimFront() and then semi.show(showNewlines=true) --";
  semi2.trimFront(); 
  semi2.show(true);

  std::cout << std::endl << "  -- semi.push_back(\"new_token\") and semi.show() -- ";
  semi2.push_back("new_token"); 
  semi2.show();

  std::cout << std::endl << "  -- semi.toLower() and semi.show() --";
  semi2.toLower();
  semi2.show();

  return 0;
}
#endif