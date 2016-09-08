#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//                                                                 //
//  Language:      Visual C++ 2008, SP1                            //
//  Application:   CSE687 Pr2                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
//  Sourse:        Prof.Jim Fawcett                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();                 //   and parse it

Build Process:
==============
Required files
TypeTable.h, TypeTable.cpp
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp

*/
//
#include <queue>
#include <string>
#include <sstream>
#include "TypeAnalysis.h"
#include "../SemiExp/itokcollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "../TypeTable/TypeTable.h"

class Repository  // application specific
{
	Scanner::Toker* p_Toker;
	TypeTable typeTable_;		//define a type table to store type info
	std::string file_;
public:
	Repository(Scanner::Toker* pToker)
	{
		p_Toker = pToker;
	}
	Scanner::Toker* Toker()
	{
		return p_Toker;
	}
	TypeTable& getTable() { return typeTable_; }		//return the typeTable
	void addRecord(TypeRecord& record) { typeTable_.addRecord(record); }	//add record
	void setFile(std::string file) { file_ = file; }	//set filepath, we need to parse it into a filename
	//parsing the full path into a filename
	std::string getFilename() 
	{ 
		//filename is int the end of the string, seperated by "\"
		size_t index = file_.find_last_of('\\');		
		std::string fileName = file_.substr(index + 1);
		return fileName;
	}
	void printTable() { typeTable_.printTable(); }
};

///////////////////////////////////////////////////////////////
// rule to detect namespace

class DetectNamespace : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("namespace") < pTc->length())
		{
			ITokCollection& tc = *pTc;
			//make sure it defines a namespace, but not uses one
			if (tc[tc.length() - 1] == "{")
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle namespace

class HandleNamespace : public IAction
{
	Repository* p_Repos;
public:
	HandleNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string type = "namespace";
		std::string name = tc[tc.find("namespace") + 1];
		std::string file = p_Repos->getFilename();
		TypeRecord tr(type, name, file);
		//add record to repo's typetable
		p_Repos->addRecord(tr);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect class

class DetectClass : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("class") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle Class

class HandleClass : public IAction
{
	Repository* p_Repos;
public:
	HandleClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string type = "class";
		std::string name = tc[tc.find("class") + 1];
		std::string file = p_Repos->getFilename();
		TypeRecord tr(type, name, file);
		p_Repos->addRecord(tr);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect struct

class DetectStruct : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("struct") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle struct

class HandleStruct : public IAction
{
	Repository* p_Repos;
public:
	HandleStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string type = "struct";
		std::string name = tc[tc.find("struct") + 1];
		std::string file = p_Repos->getFilename();
		TypeRecord tr(type, name, file);
		p_Repos->addRecord(tr);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect enum

class DetectEnum : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("enum") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle struct

class HandleEnum : public IAction
{
	Repository* p_Repos;
public:
	HandleEnum(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string type = "enum";
		std::string name = tc[tc.find("enum") + 1];
		std::string file = p_Repos->getFilename();
		TypeRecord tr(type, name, file);
		p_Repos->addRecord(tr);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect typedef

class DetectTypeDef : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("typedef") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle TypeDef

class HandleTypeDef : public IAction
{
	Repository* p_Repos;
public:
	HandleTypeDef(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string type = "typedef";
		std::string name = tc[tc.find(";") - 1];		//get name of the typedef
		std::string file = p_Repos->getFilename();
		TypeRecord tr(type, name, file);
		p_Repos->addRecord(tr);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect aliases

class DetectAliases : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("using") < pTc->length() && pTc->find("=") < pTc->length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle Aliases

class HandleAliases : public IAction
{
	Repository* p_Repos;
public:
	HandleAliases(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		//format is using XXX  = xxx;
		ITokCollection& tc = *pTc;
		std::string type = "alias";
		std::string name = tc[tc.find("using") + 1];  //get name
		std::string file = p_Repos->getFilename();
		TypeRecord tr(type, name, file);
		p_Repos->addRecord(tr);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect global function

class DetectGlobal : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		//global function start with static, and end with "{", it also includes a pair of "()"
		if (pTc->find("static") < pTc->length() && tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length())
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////
// action to handle global function

class HandleGlobal : public IAction
{
	Repository* p_Repos;
public:
	HandleGlobal(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		std::string type = "global function";
		std::string name = tc[tc.find("(") - 1];
		std::string file = p_Repos->getFilename();
		TypeRecord tr(type, name, file);
		p_Repos->addRecord(tr);
	}
};


#endif
