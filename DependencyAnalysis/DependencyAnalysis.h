#ifndef DEPENDENCYANALYSIS
#define DEPENDENCYANALYSIS
/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h - Analyzes C++ language dependency              //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a DependencyAnalysis class.  Its instances collect
semi-expressions from a file for analysis.  Not like TypeAnalysis,
instead of consists of applying a set of rules and actions to the 
semi-expression. It directly get semiExp and call check dependency to 
analyze it.
DependencyInfo is used to store dependency info, which contains a 
filename and a set of files it depends on.
*/
#include <fstream>
#include <vector>
#include "../TypeTable/TypeTable.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/itokcollection.h"

class DependencyInfo
{
public:
	void setName(std::string name);		//set filenmae
	std::string getName() { return fileName_; }
	void addDepFile(std::string file) { depFiles_.push_back(file); }	//add dependency
	std::vector<std::string>& getDepFiles() { return depFiles_; }	//return dependency vector
	void showDep();
	void removeDuplicate();		//remove duplicate keys from the vector
private:
	std::vector<std::string> depFiles_;
	std::string fileName_ = " ";  //default set as " "
};

class DependencyAnalysis
{
public:
	DependencyAnalysis(TypeTable& table);
	~DependencyAnalysis();
	void analyze();
	bool attach(std::string file);
	bool next();	//get next semi
	void checkDependency(ITokCollection& tc);	//function to check dependency
	void printDepInfo();
	void removeDuplicate();		//remove duplicate keys for di_ and dio_
	DependencyInfo& getDepInfo() { return di_; }
	DependencyInfo& getDepedInfo() { return dio_; }
private:
	std::ifstream* pIn;
	Scanner::Toker* pToker;
	Scanner::SemiExp* pSemi;
	TypeTable& table_;		//typeTable which stores all type infos
	DependencyInfo di_;    //current file's dependency info
	DependencyInfo dio_;	//stores the dependency info of the file which may depends on the current file
	std::vector<std::string> includes_;	//store's include files, but not all the include file is used
};

#endif // !DEPENDENCYANALYSIS

