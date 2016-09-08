/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
//  ver 2.1                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include "TypeAnalysis.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
	// when Builder goes out of scope, everything must be deallocated

	delete pDetectNamespace;
	delete pHandleNamesapce;
	delete pDetectClass;
	delete pHandleClass;
	delete pDetectStruct;
	delete pHandleStruct;
	delete pDetectEnum;
	delete pHandleEnum;
	delete pDetectTypeDef;
	delete pHandleTypeDef;
	delete pDetectAliases;
	delete pHandleAliases;
	delete pDetectGlobal;
	delete pHandleGlobal;
	delete pRepo;
	delete pTypeAnalysis;
	delete pSemi;
	delete pToker;
	pIn->close();
	delete pIn;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
	if (pToker == 0)
		return false;
	pRepo->setFile(name);
	pIn = new std::ifstream(name);
	if (!pIn->good())
		return false;
	return pToker->attach(pIn);
}
//----< Here's where all the parts get assembled >----------------

TypeAnalysis* ConfigParseToConsole::Build()
{
	try
	{
		// add TypeAnalysis's main parts

		pToker = new Toker;
		pToker->returnComments(false);
		pSemi = new SemiExp(pToker);
		pTypeAnalysis = new TypeAnalysis(pSemi);
		pRepo = new Repository(pToker);

		// configure to manage types
		// these must come first - they return true on match
		// so rule checking continues
		pDetectNamespace = new DetectNamespace();
		pHandleNamesapce = new HandleNamespace(pRepo);
		pDetectNamespace->addAction(pHandleNamesapce);
		pTypeAnalysis->addRule(pDetectNamespace);
		pDetectClass = new DetectClass();
		pHandleClass = new HandleClass(pRepo);
		pDetectClass->addAction(pHandleClass);
		pTypeAnalysis->addRule(pDetectClass);
		pDetectStruct = new DetectStruct();
		pHandleStruct = new HandleStruct(pRepo);
		pDetectStruct->addAction(pHandleStruct);
		pTypeAnalysis->addRule(pDetectStruct);
		pDetectEnum = new DetectEnum();
		pHandleEnum = new HandleEnum(pRepo);
		pDetectEnum->addAction(pHandleEnum);
		pTypeAnalysis->addRule(pDetectEnum);
		pDetectTypeDef = new DetectTypeDef();
		pHandleTypeDef = new HandleTypeDef(pRepo);
		pDetectTypeDef->addAction(pHandleTypeDef);
		pTypeAnalysis->addRule(pDetectTypeDef);
		pDetectAliases = new DetectAliases();
		pHandleAliases = new HandleAliases(pRepo);
		pDetectAliases->addAction(pHandleAliases);
		pTypeAnalysis->addRule(pDetectAliases);
		pDetectGlobal = new DetectGlobal();
		pHandleGlobal = new HandleGlobal(pRepo);
		pDetectGlobal->addAction(pHandleGlobal);
		pTypeAnalysis->addRule(pDetectGlobal);

		return pTypeAnalysis;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ConfigureParser module\n "
		<< std::string(32, '=') << std::endl;

	// collecting tokens from files, named on the command line

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	for (int i = 1; i < argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');

		ConfigParseToConsole configure;
		TypeAnalysis* pTypeAnalysis = configure.Build();
		try
		{
			if (pTypeAnalysis)
			{
				if (!configure.Attach(argv[i]))
				{
					std::cout << "\n  could not open file " << argv[i] << std::endl;
					continue;
				}
			}
			else
			{
				std::cout << "\n\n  TypeAnalysis not built\n\n";
				return 1;
			}
			// now that parser is built, use it

			while (pTypeAnalysis->next())
				pTypeAnalysis->analyze();
			std::cout << "\n\n";
		}
		catch (std::exception& ex)
		{
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		std::cout << "\n\n";
	}
}

#endif
