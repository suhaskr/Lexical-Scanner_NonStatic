/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a file stream                   //
// ver 3.5                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Suhas Kamasetty Ramesh, Syracuse University        //
//              skamaset@syr.edu                                   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////

/*
* Public Interface: (For methods of ConsumeState Class)
* -------------------------------------------------------
*
*  std::string getTok() - returns one token as a string(eg. alphanum or special character etc.)
*
*  void setSpecialSingleChars(std::string ssc) - There is a static vector containing a list of Single Character
*                                                 tokens. We can add new characters using this function.
*
*  void setSpecialCharPairs(std::string scp) - There is a static vector containing a list of Character Pair tokens.
*                                              We can add new character pairs to the vector using this function.
*
*  void printSpecialTokens() - Prints the list of Special Single and Double character tokens.
*
*  void attach(std::istream* pIn) - Attaches the filestream  pointer to ConsumeState variable.
*
*  void eatChars() = 0; - Pure virtual function that will be implemented by the derived classes of ConsumeState Class
*
*  bool canRead() - Check whether characters are avilable to read from file/string stream.
*
*  void consumeChars() - Eats characters sequentially in the present state and move to next state based on the next character.
*  
*
*
*  Package Operations:
*  This package defines an abstract ConsumeState class. Derived classes EatAlphaNum,
*  EatPunctuator, EatSingleSpecialCharacter etc implement the ConsumeState class.
*  The derived classes provide implementation for hte eatChars() function.
*  
*/

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>

namespace Scanner
{
  ///////////////////////////////////////////////////////////////////
  // Context is a shared data storage facility.
  /*
  * - with Context we don't need static members to share
  *   state pointes etc.  Context holds and shares all that.
  * - Context is an entirely private facility used by ConsumeState
  *   and its derived instances, so there is no need for encapsulation
  *   and we use a struct.  This won't be appropriate if we handed
  *   the context to clients of Toker.
  */

  struct Context {
	Context();
	~Context();
	std::string token;
	std::istream* _pIn;
	std::vector<std::string> _oneCharTokens = 
	{ 
	  "<", ">", "[", "]", "(", ")", "{", "}", ":", "=", "+", "-", "\n" ,"*" 
	};

	std::vector<std::string> _twoCharTokens = 
	{ 
	  "<<", ">>", "::", "++", "==", "+=", "-=", "*=", "/=" 
	};

	int prevChar;
	int currChar;
	int nextChar;
	bool _doReturnComments;
	size_t _lineCount;
	ConsumeState* _pState;
	ConsumeState* _pEatCppComment;
	ConsumeState* _pEatCComment;
	ConsumeState* _pEatWhitespace;
	ConsumeState* _pEatPunctuator;
	ConsumeState* _pEatAlphanum;
	ConsumeState* _pEatSpecialOneChar;
	ConsumeState* _pEatSpecialTwoChar;
	ConsumeState* _pEatDQString;
	ConsumeState* _pEatSQString;
  };


  ///////////////////////////////////////////////////////////////////
  // private ConsumeState worker class
  /*
  *   Nota Bene:
  *   - At exit the eatChars() member should return with
  *     currChar set to the first non-state character.
  *   - Each derived class must ensure that this condition
  *     is meet.
  *
  *  This ConsumeState class is nearly identical to the
  *  the ConsumeState that uses static member sharing.
  *  We've just replaced the static members with references
  *  to those members stored in the context.
  */

  class ConsumeState
  {

  public:
    ConsumeState();
    ConsumeState(const ConsumeState&) = delete;
    ConsumeState& operator=(const ConsumeState&) = delete;
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pContext->_pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
      _pContext->_pState->eatChars();
      _pContext->_pState = nextState();
    }
    bool canRead() { return _pContext->_pIn->good(); } //See if more characters are present in file stream.
    std::string getTok() { return _pContext->token; }  //Returns a token
    bool hasTok() { return _pContext->token.size() > 0; } //If no characters are added to string token, it returns false
    ConsumeState* nextState();
	ConsumeState* nextFewStates();
	void returnComments(bool doReturnComments = false); 
	size_t currentLineCount();
	void setContext(Context* pContext);

	//void reset();
	void setSpecialSingleChars(std::string ssc) { _pContext->_oneCharTokens.push_back(ssc); }
	void setSpecialCharPairs(std::string scp) { _pContext->_twoCharTokens.push_back(scp); }
	void printTokens() {
		std::cout << "\n  Single character tokens:" << std::endl << "  ";
		for (auto tok : _pContext->_oneCharTokens) {
			if (tok == "\n")
				tok = "newline";
			std::cout << tok << " ";
		}
		std::cout << "\n  Two character tokens:" << std::endl << "  ";
		for (auto tok : _pContext->_twoCharTokens)
			std::cout << tok << " ";
		std::cout << std::endl;
	}

  protected:
	Context* _pContext;
	//bool collectChar();
	//bool isOneCharToken(Token tok);
	//bool isTwoCharToken(Token tok);
  };
}

using namespace Scanner;


//----< logs to console if TEST_LOG is defined >---------------------
void testLog(const std::string& msg);


//----< used by Toker to provide Consumer with Context ptr >---------
void ConsumeState::setContext(Context* pContext)
{
  _pContext = pContext;
}


//----< return number of newlines collected from stream >------------
size_t ConsumeState::currentLineCount() { return _pContext->_lineCount; }


//----< turn on\off returning comments as tokens >-------------------
void ConsumeState::returnComments(bool doReturnComments)
{ 
  _pContext->_doReturnComments = doReturnComments;  //Option to return comments as tokens or not. 
} 


//-------< Determine the next state to enter based on the current and next characters >---------------------------
ConsumeState* ConsumeState::nextState()
{
  if (!(_pContext->_pIn->good()))
  {
	  //std::cout << "=====================";
    return nullptr;
  }
  _pContext->nextChar = _pContext->_pIn->peek();
  std::string temp = "";
  temp += _pContext->currChar;
  temp += _pContext->nextChar;
  if (_pContext->nextChar == EOF)
  {
    _pContext->_pIn->clear();
	//std::cout << "**************************************";
    // if peek() reads end of file character, EOF, then eofbit is set and
    // _pIn->good() will return false.  clear() restores state to good
  }
  if (std::isspace(_pContext->currChar) && _pContext->currChar != '\n')
  {
    testLog("state: eatWhitespace");
    return _pContext->_pEatWhitespace;
  }
  if (_pContext->currChar == '/' && _pContext->nextChar == '/')
  {
    testLog("state: eatCppComment");
    return _pContext->_pEatCppComment;
  }
  if (_pContext->currChar == '/' && _pContext->nextChar == '*')
  {
    testLog("state: eatCComment");
    return _pContext->_pEatCComment;
  }
  /*
  if (currChar == '\n')
  {
    testLog("state: eatNewLine");
    return _pEatNewline;
  }*/
  if (std::isalnum(_pContext->currChar) || (_pContext->currChar == '_' && isalnum(_pContext->nextChar)))
  {
    testLog("state: eatAlphanum");
    return _pContext->_pEatAlphanum;
  }
  
  return nextFewStates();
 
}

//-------< Determine the next state to enter based on the current and next character >---------------------------
ConsumeState* ConsumeState::nextFewStates() {
	std::string temp = "";
	temp += _pContext->currChar;
	temp += _pContext->nextChar;
	if (_pContext->currChar == '"')
	{
		testLog("state: eatDoubleQuotedString");
		return _pContext->_pEatDQString;
	}
	if (_pContext->currChar == '\'') {
		testLog("state: eatSingleQuotedString");
		return _pContext->_pEatSQString;
	}


	for (auto str : _pContext->_twoCharTokens) {
		if (str == temp) {
			testLog("state: eatSpecialCharacterPair");
			return _pContext->_pEatSpecialTwoChar;
		}
	}
	for (auto str : _pContext->_oneCharTokens) {
		if (str[0] == _pContext->currChar) {
			testLog("state: eatSpecialSingleCharacter");
			return _pContext->_pEatSpecialOneChar;
		}
	}

	if (ispunct(_pContext->currChar))
	{
		testLog("state: eatPunctuator");
		return _pContext->_pEatPunctuator;
	}
	if (!_pContext->_pIn->good())
	{
		std::cout << "\n  end of stream with currChar = " << _pContext->currChar << "\n\n";
		return _pContext->_pEatWhitespace;
	}
	throw(std::logic_error("invalid type"));
}


//-------< Class to eat white space characters. >---------------------------
//Note: (new lines are handled using special single character class)
class EatWhitespace : public ConsumeState
{
public:
  EatWhitespace(Context* pContext) 
  {
	_pContext = pContext;
  }

  virtual void eatChars()
  {
    _pContext->token.clear();
    //std::cout << "\n  eating whitespace";
    do {
      if (!_pContext->_pIn->good())  // end of stream
        return;
      _pContext->currChar = _pContext->_pIn->get();
    } while (std::isspace(_pContext->currChar) && _pContext->currChar != '\n');
  }
};


//-------< Class to eat CPP styled comments >-------------------------
//Note: By default comments are discarded.
//  An option is provided to return comments. (use returnComments method of Toker class to do that)
class EatCppComment : public ConsumeState
{
public:
	EatCppComment(Context* context) 
	{
		_pContext = context;
	}
  virtual void eatChars()
  {
    _pContext->token.clear();
    //std::cout << "\n  eating C++ comment";
    do {
		if(_pContext->_doReturnComments)
			_pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
      _pContext->currChar = _pContext->_pIn->get();
    } while (_pContext->currChar != '\n');
  }
};


//-------< Class to eat C styled comments >-------------------------
//Note: By default comments are discarded.
//  An option is provided to return comments. (use returnComments method of Toker class to do that)
class EatCComment : public ConsumeState
{
public:
	EatCComment(Context* context) 
	{
		_pContext = context;
	}

  virtual void eatChars()
  {
    _pContext->token.clear();
    //std::cout << "\n  eating C comment";
    do {
		if(_pContext->_doReturnComments)
		  _pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
      _pContext->currChar = _pContext->_pIn->get();
    } while (_pContext->currChar != '*' || _pContext->_pIn->peek() != '/');
	if(_pContext->_doReturnComments)
	  _pContext->token += _pContext->currChar;
    _pContext->currChar = _pContext->_pIn->get();
	if(_pContext->_doReturnComments)
	  _pContext->token += _pContext->currChar;
    _pContext->currChar = _pContext->_pIn->get();
  }
};


//-------< Class to eat all punctuators >-------------------------
class EatPunctuator : public ConsumeState
{
public:
	EatPunctuator(Context* context) 
	{
		_pContext = context;
	}

  virtual void eatChars()
  {
    _pContext->token.clear();
    //std::cout << "\n  eating punctuator";
    do {
      _pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
    } while (ispunct(_pContext->currChar));
  }
};


//-------< Class to eat alphabets and numbers >-------------------------
//Note: If we have an '_' while eating alphanum, we do not change state to punctuator,
//       we add the '_' to token and go ahead to eat other alphanum characters.
class EatAlphanum : public ConsumeState
{
public:
	EatAlphanum(Context* context) 
	{
		_pContext = context;
	}
  virtual void eatChars()
  {
	_pContext->token.clear();
    //std::cout << "\n  eating alphanum";
    do {
	  _pContext->token += _pContext->currChar;
      if (!_pContext->_pIn->good())  // end of stream
        return;
	  _pContext->currChar = _pContext->_pIn->get();
    } while (isalnum(_pContext->currChar) || _pContext->currChar == '_');
	//|| currChar == '_'
  }
};


//-------< Class to eat new lines >-------------------------
// This class is not used anymore as new lines are taken care by Eat Special Single character class. 
class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
	_pContext->token.clear();
    //std::cout << "\n  eating new line";
	_pContext->token += _pContext->currChar;
    if (!_pContext->_pIn->good())  // end of stream
      return;
	_pContext->currChar = _pContext->_pIn->get();
  }
};


//-------< Class to eat speacial character pair>-------------------------
class EatSpecialTwoChar : public ConsumeState 
{
public:
	EatSpecialTwoChar(Context* context) 
	{   
		_pContext = context;
	}
	virtual void eatChars() 
	{
	    _pContext->token.clear();
		//std::cout << "\n  eating special character pair";
		_pContext->token += _pContext->currChar;
		if (!_pContext->_pIn->good())
			return;
		_pContext->currChar = _pContext->_pIn->get();
		_pContext->token += _pContext->currChar;
		if (!_pContext->_pIn->good())
			return;
		_pContext->currChar = _pContext->_pIn->get();
	}
};

//-------< Class to eat single speacial character>-------------------------
class EatSpecialOneChar : public ConsumeState 
{
public:
	EatSpecialOneChar(Context* context) 
	{
		_pContext = context;
	}
	virtual void eatChars() {
	    _pContext->token.clear();
		//std::cout << "\n  eating special single character";
		_pContext->token += _pContext->currChar;
		if (!_pContext->_pIn->good())
			return;
		_pContext->currChar = _pContext->_pIn->get();
	}
};

//-------< Class to eat double quoted strings>-------------------------
// Note: The outer quotes are discarded.
//       If we come accross \" we replace it with a "
class EatDQString : public ConsumeState 
{
public:
	EatDQString(Context* context) {
		_pContext = context;
	}

	virtual void eatChars() {
	    _pContext->token.clear();
		//std::cout << "\n  eating double quoted strings";
		_pContext->prevChar = _pContext->currChar;
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		while(!(_pContext->prevChar != '\\' && _pContext->currChar == '\"'))
		{
			//if (!(currChar == '\\' && _pIn->peek() == '\"'))
			_pContext->token += _pContext->currChar;
			_pContext->prevChar = _pContext->currChar;
			if (!_pContext->_pIn->good())
				return;
			_pContext->currChar = _pContext->_pIn->get();
		}
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
	}
};

//-------< Class to eat singlee quoted strings>-------------------------
// Note: The outer quotes are discarded.
//       If we come accross \' we replace it with '
class EatSQString : public ConsumeState 
{
public:
	EatSQString(Context* context) 
	{
		_pContext = context;
	}
	virtual void eatChars() {
		_pContext->token.clear();
		//std::cout << "\n  eating single quoted strings";
		_pContext->prevChar = _pContext->currChar;
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		while (!(_pContext->prevChar != '\\' && _pContext->currChar == '\'')) {
			//if(!(_pContext->currChar == '\\' && _pContext->_pIn->peek() == '\''))
				_pContext->token += _pContext->currChar;
			_pContext->prevChar = _pContext->currChar;
			if (!_pContext->_pIn->good())
				return;
			_pContext->currChar = _pContext->_pIn->get();
		}
		_pContext->token += _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
	}
};


//----< construct shared data storage >------------------------------
Context::Context() 
{
	_pEatAlphanum = new EatAlphanum(this);
	_pEatCComment = new EatCComment(this);
	_pEatCppComment = new EatCppComment(this);
	_pEatWhitespace = new EatWhitespace(this);
	_pEatPunctuator = new EatPunctuator(this);
	_pEatSpecialOneChar = new EatSpecialOneChar(this);
	_pEatSpecialTwoChar = new EatSpecialTwoChar(this);
	_pEatDQString = new EatDQString(this);
	_pEatSQString = new EatSQString(this);	
	_pState = _pEatWhitespace;
	_lineCount = 1;
	_doReturnComments = false;
}


//----< return shared resources >------------------------------------
Context::~Context() {
	delete _pEatAlphanum;
	delete _pEatCComment;
	delete _pEatCppComment;
	delete _pEatWhitespace;
	delete _pEatPunctuator;
	delete _pEatSpecialOneChar;
	delete _pEatSpecialTwoChar;
	delete _pEatDQString;
	delete _pEatSQString;
}


//-------< Constructor of ConsumeState class>-------------------------
//----< no longer has anything to do - will be removed >-------------
ConsumeState::ConsumeState() { }


//-------< Destructor of ConsumeState class>-------------------------
//----< no longer has anything to do - will be removed >-------------
ConsumeState::~ConsumeState() { }


//-------< Constructor of Toker class>-------------------------
//----< construct toker starting in state EatWhitespace >------------
Toker::Toker()
{
	_pContext = new Context();
	pConsumer = _pContext->_pEatWhitespace;
	pConsumer->setContext(_pContext);
}


//----< Destructor of Toker class >-----------------------
//----< it deletes the created context which has all the different states >------------
Toker::~Toker()
{
	delete _pContext;
	_pContext = nullptr;
	//pConsumer = nullptr;
}


//----< attach tokenizer to stream >---------------------------------
/*
* stream can be either a std::fstream or std::stringstream
*/
bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
	//_pContext->_pIn = pIn;
    return true;
  }
  return false;
}


//-------< Eat characters in a particular state.>-------------------------
// If a token is present it returns the token an comes out of the loop.
// If there is not token, it moves to next state to eat characters.
std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
	//std::cout << "\n  No token";
  }
  return pConsumer->getTok();
}

//-------< See if there are mor character to be read from file.>-------------------------
bool Toker::canRead() { return pConsumer->canRead(); }

//-------< Add string to list of special single characters>-------------------------
void Toker::setSpecialSingleChars(std::string ssc) {
	pConsumer->setSpecialSingleChars(ssc);
}

//-------< Add string to list of special character pairs>-------------------------
void Toker::setSpecialCharPairs(std::string scp) {
	pConsumer->setSpecialCharPairs(scp);
}

//-------< Print the list of special single character and character pairs>-------------------------
void Toker::printSpecialTokens() {
	pConsumer->printTokens();
}

//-------< An option to return comments or not>-------------------------
// Note: By default comments are discarded and not returned as tokens.
void Toker::returnComments(bool doReturnComments) {
	pConsumer->returnComments(doReturnComments);
}


//----< return number of newlines pulled from stream >---------------
size_t Toker::currentLineCount()
{
	return pConsumer->currentLineCount();
}


void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}
//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
  testLog("Yes");
  //std::cout << "hello\n";
  //std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  //std::string fileSpec = "../Tokenizer/Tokenizer.h";
  //std::string fileSpec = "../Tokenizer/Test2.txt";
  std::string fileSpec = "../TestFiles/TokenizerTest2.txt";


  std::ifstream in(fileSpec);
  //in.
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
  }
  Toker toker;
  toker.printSpecialTokens();
  toker.returnComments(true);
 // toker.setSpecialSingleChars("*");
  //toker.setSpecialCharPairs("<=");
  std::vector<std::string> abc = { "One", "Two", "Three" };
  abc.push_back("a");
  for (auto st : abc) {
	  //if (st == "a")
		  std::cout << "vector abc contains: " << st;
  }
  toker.attach(&in);
  do
  {
    std::string tok = toker.getTok();
    if (tok == "\n")
      tok = "newline";
    std::cout << "\n -- " << tok;
  } while (in.good());

  std::cout << "\n\n";
  return 0;
}
#endif