////////////////////////////////////////////////////////////////////////////////
//  ParallelDepAnalysisExecutive.cpp - Analyzes Dependency of c++ files       //
//  ver 1.4                                                                   //
//  Language:      Visual C++                                                 //
//  Application:   CSE687 Pr3                                                 //
//  Author:        Yingxuan Wang                                              //
//                 ywang206@syr.edu                                           //
////////////////////////////////////////////////////////////////////////////////
/*
Executive for parser.

Build Process :
====================
Required files
- IFileMgr.h, IFileMgr.cpp, FileMgr.h, FileMgr.cpp
- Logger.h, Logger.cpp
- Utilities.h, Utilities.cpp
- ConfigureParser.h, ConfigureParser.cpp
- TypeAnalysis.h, TypeAnalysis.cpp
- Tasks.h, Tasks.cpp
- DependencyAnalysis.h, DependencyAnalysis.h
- ParallelDepAnalysis.h, ConfigureParser.cpp
- TypeTable.h, TypeTable.cpp
- ThreadPool.h

====================
Command: Parser path *.pat1,*.pat2
*/

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <future>
#include "../FileMgr/IFileMgr.h"
#include "../FileMgr/FileMgr.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include "../TypeAnalysis/ConfigureParser.h"
#include "../TypeAnalysis/TypeAnalysis.h"
#include "../Tasks/Tasks.h"
#include "../DependencyAnalysis/DependencyAnalysis.h"
#include "../ParallelDependencyAnalysis/ParallelDepAnalysis.h"

class Executive
{
public:
	Executive(int argc, char* argv[]);
	void startAnalysis();
	bool parseCommand(int argc, char* argv[]);
private:
	std::string path_;
	std::string filePatterns_;
};

Executive::Executive(int argc, char* argv[])
{
	std::cout << "\n  ----------- Run Parallel Dependency Analysis Tool -----------";
	if (!parseCommand(argc, argv))
		return;
}

//parse command line
bool Executive::parseCommand(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cout << "\n  Wrong arguments!";
		std::cout << "\n  Expected command format: AnalysisExecutive path *.pat1,*.pat2 ! \n\n";
		return false;
	}
	path_ = argv[1];
	filePatterns_ = argv[2];
	std::cout << "\n  Path: " << path_ << "; file patterns: " << filePatterns_;
	return true;
}

//start analysis files
void Executive::startAnalysis()
{
	ParallelDepAnalysis pda(path_, filePatterns_);
	//first do type analysis
	pda.typeAnalysis();
	//then do dependency analysis
	pda.depAnalysis();
	//send stop to thread pool
	pda.stop();
}

#ifdef  TEST_ANALYSIS

int main(int argc, char* argv[])
{
	Executive exe(argc, argv);
	exe.startAnalysis();
	std::cout << "\n";
}
#endif