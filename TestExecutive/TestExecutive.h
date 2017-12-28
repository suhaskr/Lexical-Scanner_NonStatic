#ifndef TESTEXECUTIVE_H
#define TESTEXECUTIVE_H
///////////////////////////////////////////////////////////////////////
// TestExecutive.h - Automated test suite for tokenizer and          // 
//                     semiexpresion packages.                       //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Demonstrate requirements for Project-1 of OOD-CSE687 //
// Author:      Suhas Kamasetty Ramesh, Syracuse University          //
//              skamaset@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
*
* Package Operations:
* -------------------
* This package demonstrates all the requirements for Project-1 of CSE-687
* It is an automated test suite that tests both the semiexpression and tokenizer packages.
*
*  Public Interface:
*  ---------------------------------
* TestR3(), TestR4(), TestR5(), TestR678(), TestR9() demonstrate the corresponding requirements 
*   of the Project-1 Statement
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
* ver 1.0 : 07 Feb 2016
* - first release
*/

#include <string>

class TestExec {
public:
	void printTitle(std::string title, char underline = '-');
	void TestR3();
	void TestR4();
	void TestR4a();
	void TestR5();
	void TestR678();
	void TestR9();
};

#endif
