#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a file stream                       //
// ver 3.5                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
//                                                                   //
// Note: This tokenizer uses a ConsumeState class with no statics.   //
//       You should prefer this over the statics-based Tokenizer.    //
//                                                                   //
//                                                                   //
// Author:      Suhas Kamasetty Ramesh, Syracuse University          //
//              skamaset@syr.edu                                     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 *
 * Public Interface:
 * -----------------
 *  bool attach(std::istream* pIn) - Attach pointer of file stream to ConsumeState pointer.
 *
 *  std::string getTok() - returns one token as a string(eg. alphanum or special character etc.)
 *
 *  bool canRead() - Check whether characters are available to read from file stream.
 *
 *  void setSpecialSingleChars(std::string ssc) - Add a particular string to the default list of
 *                                                Single Character tokens.
 *
 *  void setSpecialCharPairs(std::string scp) - Add a particular string to the default list of
 *                                              Double Character tokens.
 *
 *  void printSpecialTokens() - Prints the list of Special Single and Double character tokens.
 *
 *  void returnComments(bool doReturnComments = false) - Gives an option to user to return comments.
 *                                               
 *
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Project1.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 3.6 : 22 Feb 2016
 * - Converted tokenizer into a non-static context based one.
 * - 
 * ver 3.5 : 07 Feb 2016
 * - Added methods to set Special Single and Double character tokens
 * - Added new classes to eat special single and double character tokens
 * - Added option to return C and CPP comments
 * ver 3.4 : 03 Feb 2016
 * - fixed bug that prevented reading of last character in source by
 *   clearing stream errors at beginning of ConsumeState::nextState()
 * ver 3.3 : 02 Feb 2016
 * - declared ConsumeState copy constructor and assignment operator = delete
 * ver 3.2 : 28 Jan 2016
 * - fixed bug in ConsumeState::nextState() by returning a valid state
 *   pointer if all tests fail due to reaching end of file instead of
 *   throwing logic_error exception.
 * ver 3.1 : 27 Jan 2016
 * - fixed bug in EatCComment::eatChars()
 * - removed redundant statements assigning _pState in derived eatChars() 
 *   functions
 * - removed calls to nextState() in each derived eatChars() and fixed
 *   call to nextState() in ConsumeState::consumeChars()
 * ver 3.0 : 11 Jun 2014
 * - first release of new design
 *
 */
#include <iosfwd>
#include <string>

namespace Scanner
{
  class ConsumeState;   // private worker class
  struct Context;       // private shared data storage

  class Toker
  {
  public:
    Toker();	
    Toker(const Toker&) = delete;
    ~Toker();
    Toker& operator=(const Toker&) = delete;
    bool attach(std::istream* pIn);
    std::string getTok();
    bool canRead();
	void setSpecialSingleChars(std::string ssc);
	void setSpecialCharPairs(std::string scp);
	void printSpecialTokens();
	void returnComments(bool doReturnComments = false);
	//isComment()
	size_t currentLineCount();

  private:
    ConsumeState* pConsumer;
	Context* _pContext;
  };
}
#endif