#ifndef TYPEANALYSIS_H
#define TYPEANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.h - Analyzes C++ language types                   //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
//  Sourse:        Prof.Jim Fawcett                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a TypeAnalysis class.  Its instances collect
semi-expressions from a file for analysis.  Analysis consists of
applying a set of rules to the semi-expression, and for each rule
that matches, a set of actions is invoked.
It basically uses the design and structure of parser.
*/ 

#include <string>
#include <iostream>
#include <vector>
#include "../SemiExp/itokcollection.h"

///////////////////////////////////////////////////////////////
// abstract base class for parsing actions
//   - when a rule succeeds, it invokes any registered action

class TypeAnalysis;

class IBuilder
{
public:
	virtual ~IBuilder() {}
	virtual TypeAnalysis* Build() = 0;
};

///////////////////////////////////////////////////////////////
// abstract base class for parsing actions
//   - when a rule succeeds, it invokes any registered action

class IAction
{
public:
	virtual ~IAction() {}
	virtual void doAction(ITokCollection*& pTc) = 0;
};

///////////////////////////////////////////////////////////////
// abstract base class for parser language construct detections
//   - rules are registered with the parser for use

class IRule
{
public:
	virtual ~IRule() {}
	void addAction(IAction* pAction);
	void doActions(ITokCollection*& pTc);
	virtual bool doTest(ITokCollection*& pTc) = 0;
protected:
	std::vector<IAction*> actions;
};

class TypeAnalysis
{
public:
	TypeAnalysis(ITokCollection* pTokCollection);
	~TypeAnalysis();
	void addRule(IRule* pRule);
	bool analyze();
	bool next();
private:
	ITokCollection* pTokColl;
	std::vector<IRule*> rules;
};

inline TypeAnalysis::TypeAnalysis(ITokCollection* pTokCollection) : pTokColl(pTokCollection) {}

inline TypeAnalysis::~TypeAnalysis() {}

#endif