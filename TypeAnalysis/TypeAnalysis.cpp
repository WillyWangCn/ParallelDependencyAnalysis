/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.cpp - Analyzes C++ language types                 //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "TypeAnalysis.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
using namespace Utilities;

//----< register parsing rule >--------------------------------

void TypeAnalysis::addRule(IRule* pRule)
{
	rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool TypeAnalysis::next()
{
	// default operation of doFold(pTokColl) is to simply return
	// - code analysis provides specific rules for handling
	//   for statements and operator expressions which are
	//   bound in ConfigureParser.cpp

	bool succeeded = pTokColl->get();
	if (!succeeded)
		return false;
	return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool TypeAnalysis::analyze()
{
	bool succeeded = false;
	for (size_t i = 0; i < rules.size(); ++i)
	{
		if (rules[i]->doTest(pTokColl))
			succeeded = true;
	}
	return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
	actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
	if (actions.size() > 0)
		for (size_t i = 0; i < actions.size(); ++i)
			actions[i]->doAction(pTokColl);
}

//----< test stub >--------------------------------------------

#include "../FileMgr/FileSystem.h"

#ifdef TEST_TYPEANALYSIS

#include <queue>
#include <string>
#define Util StringHelper

int main(int argc, char* argv[])
{
	Util::Title("Testing TypeAnalysis Class", '=');
	putline();

	// collecting tokens from files, named on the command line

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	for (int i = 1; i < argc; ++i)
	{
		std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
		std::string msg = "Processing file" + fileSpec;
		Util::title(msg);
		putline();

		ConfigParseToConsole configure;
		TypeAnalysis* pTypeAnalysis = configure.Build();
		try
		{
			if (pTypeAnalysis)
			{
				if (!configure.Attach(argv[i]))
				{
					std::cout << "\n  could not open file " << fileSpec << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  TypeAnalysis not built\n\n";
				return 1;
			}
			// now that TypeAnalysis is built, use it

			while (pTypeAnalysis->next())
				pTypeAnalysis->analyze();
			std::cout << "\n";

			std::cout << "\n Print table:";
			configure.printTable();
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
	}
	std::cout << "\n";
}

#endif