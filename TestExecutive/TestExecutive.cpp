///////////////////////////////////////////////////////////////////////
// TestExecutive.h - Automated test suite for tokenizer and          // 
//                     semiexpresion packages.                       //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Demonstrate requirements for Project-1 of OOD-CSE687 //
// Author:      Suhas Kamasetty Ramesh, Syracuse University          //
//              skamaset@syr.edu                                     //
///////////////////////////////////////////////////////////////////////

#include "../SemiExp/SemiExp.h"
#include "TestExecutive.h"
#include <iostream>
#include <fstream>


//-------< Helper function to display titles. >----------------
void TestExec::printTitle(std::string title, char underline) {
	std::cout << "  " << title << std::endl << "  ";
	for (size_t i = 0; i <= title.length(); i++)
		std::cout << underline;
	std::cout << std::endl;
}

//-------< Demonstrate Requirement-3 >----------------
void TestExec::TestR3() {
	std::cout << std::endl;
	printTitle("Demonstrating Requirement #3");
	std::cout << "  Created three packages in the Project1 solution as below:" << std::endl;
	std::cout << "   Package Tokenizer for tokenizing input file stream or string stream" << std::endl;
	std::cout << "   Package SemiExp to build semi expressions using tokens returned from tokenizer" << std::endl;
	std::cout << "   Package TestExecutive to demonstrate all the requirements for Project-1" << std::endl;
}

//-------< Demonstrate Requirement-4 >----------------
void TestExec::TestR4() {
	std::cout << std::endl;
	printTitle("Demonstratin Requirement #4");
	std::cout << "  Declared and defined a Toker class that implements the State Pattern." << std::endl;
	std::cout << "  Uses abstract ConsumeState class and derived classes for collecting different types of tokens" << std::endl;
	std::cout << "\n  Going to tokenize the file TokenizerTest1.txt which includes alphanum, punctuator and special character tokens";
	Scanner::Toker toker1;
	std::fstream in1("./../TestFiles/TokenizerTest1.txt");
	if (!in1.good()) {
		std::cout << "could not open file: TokenizerTest1.txt";
		return;
	}
	toker1.attach(&in1);
	do
	{
		std::string tok = toker1.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in1.good());
	std::cout << "\n\n  The default Single and Double character tokens are as shown below:";
	toker1.printSpecialTokens();
	std::cout << "\n  Added <= and >= to the double characeter tokens list and $ to Single character tokens list" << std::endl;
	std::cout << "  using setSpecialCharPairs and setSpecialSingleChars functions of Tokenizer." << std::endl;
	toker1.setSpecialCharPairs("<=");
	toker1.setSpecialCharPairs(">=");
	toker1.setSpecialSingleChars("$");
	std::cout << "  The Single and Double character tokens are as shown below:";
	toker1.printSpecialTokens();	
	toker1.reset();
	TestR4a();
	std::cout << "\n\n\n  Going to tokenize the file TokenizerTest2.txt which include C  and CPP Comments and quoted strings.";
	Scanner::Toker toker2;
	std::fstream in2("./../TestFiles/TokenizerTest2.txt");
	if (!in2.good()) {
		std::cout << "could not open file: TokenizerTest2.txt";
		return;
	}
	toker2.attach(&in2);
	toker2.returnComments(true);
	do {
		std::string tok = toker2.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in2.good());
	std::cout << std::endl;
	toker2.reset();
}

//-------< Demonstrate second part of Requirement-4 >----------------
void TestExec::TestR4a() {
	std::cout << "\n  Now going to run the tokenizer again on the file." << std::endl;
	std::cout << "  Now observe that <= come as a single token and $ comes as a single token:";
	Scanner::Toker toker5;
	std::fstream in5("./../TestFiles/TokenizerTest1.txt");
	if (!in5.good()) {
		std::cout << "could not open file: TokenizerTest1.txt";
		return;
	}
	toker5.attach(&in5);
	do
	{
		std::string tok = toker5.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in5.good());
	toker5.reset();
}
//-------< Demonstrate Requirement-5 >----------------
void TestExec::TestR5() {
	std::cout << std::endl;
	printTitle("Demonstrating Requirement #5");
	std::cout << "  The member function getTok() of the Tokenizer package returns exactly one token." << std::endl;
	std::cout << "  The tokens shown above were extracted from the file stream by calling the \n  toker.getTok() method of Tokenizer package multiple times until eof is reached" << std::endl;
}

//-------< Demonstrate Requirements- 6, 7 and 8 >----------------
void TestExec::TestR678() {
	std::cout << std::endl;
	printTitle("Demonstrating Requirements #6, #7 and #8");
	std::cout << "  Created a SemiExpression package that retrieves collections of tokens by calling the" << std::endl;
	std::cout << "  Tokker::getTok() method repeatedly until the SemiExp termination conditions is satisfied:" << std::endl;
	std::cout << "  Created an object of the SemiExp class and ran it on the SemiExpressionTest1.txt file.\n  The semi expressions are shown below:" << std::endl;
	
	Scanner::Toker toker3;
	std::fstream in3("./../TestFiles/SemiExpressionTest1.txt");
	if (!in3.good()) {
		std::cout << "could not open file: SemiExpressionTest1.txt";
		return;
	}
	toker3.attach(&in3);
	toker3.returnComments(false);
	Scanner::SemiExp semi(&toker3);

	while (semi.get()) {
		std::cout << "\n  -- semiExpression --";
		semi.show();
	}
	toker3.reset();
}

//-------< Demonstrate Requirement-9 >----------------
void TestExec::TestR9() {
	std::cout << std::endl;
	printTitle("Demonstrating Requirement #9");
	std::cout << "  Going to work on the code present in SemiExpressionTest2.txt" << std::endl;
	std::cout << "  The code in the file is as shown below:\n  newline newline string ABC = \"This IS A STring OBJECT\";" << std::endl;
	Scanner::Toker toker4;
	std::fstream in4("./../TestFiles/SemiExpressionTest2.txt");
	if (!in4.good()) {
		std::cout << "could not open file: SemiExpressionTest2.txt";
		return;
	}
	toker4.attach(&in4);
	Scanner::SemiExp semi2(&toker4);	
	std::cout << "  Will call all the functions of the interface ITokcollection and display the output:" << std::endl;
	std::cout << std::endl << "  -- semi.get(clear=true) --" << std::endl ;
	std::cout << "      " << semi2.get(true);
	std::cout <<  "\n  semi.get() will store a semiexpression(tokens) in a vector of string and returns true.\n  If not semi expression is generated it returns false" << std::endl;
	std::cout << std::endl << "  -- semi.show(showNewLines=false) -- ";
	semi2.show(false);
	std::cout << std::endl << "  -- semi.show(showNewlines=true) --" ;
	std::string returnOfShow = semi2.show(true);
	std::cout << std::endl << "  Thre return value of show is a concatenated string of tokens in semi expression as shown below:" << std::endl;
	std::cout << "  " << returnOfShow << std::endl;
	std::cout << std::endl << "  -- semi.trimFront() and then semi.show(showNewlines=true) --";
	semi2.trimFront(); semi2.show(true);
	std::cout << std::endl << "  -- semi.push_back(\"new_token\") and semi.show() -- ";
	semi2.push_back("new_token"); semi2.show();
	std::cout << std::endl << "  -- semi.toLower() and semi.show() --";
	semi2.toLower();
	semi2.show();
	std::cout << std::endl << "  -- semi.length() --";
	std::cout << "\n          " << semi2.length();
	std::cout << std::endl << "  -- semi.find(\"new_token\") --";
	std::cout << "\n          " << semi2.find("new_token");
	std::cout << std::endl << "  -- semi.find(;) --";
	std::cout << "\n          " << semi2.find(";");
	std::cout << std::endl << "  -- semi.remove(size_t 4) (will remove ;) and semi.show() --";
	semi2.remove(4);
	semi2.show();
	std::cout << std::endl << "  -- semi.remove(\"new_token\") and semi.show() --";
	semi2.remove("new_token");
	semi2.show();
	std::cout << std::endl << "  -- semi[2] --";
	std::cout << "\n        " << semi2[2] << std::endl;
	std::cout << std::endl << "  -- semi.clear() and semi.show()--";
	semi2.clear();
	semi2.show();
	std::cout << "  Since there are no tokens in the semi expression nothing is displayed.";
}

int main() {
	TestExec exec;
	exec.printTitle("Demonstrating Project#1 Requirements",'=');
	exec.TestR3();
	exec.TestR4();
	exec.TestR5();
	exec.TestR678();
	exec.TestR9();
	return 0;
}
