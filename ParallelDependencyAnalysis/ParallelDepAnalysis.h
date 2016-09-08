#ifndef PARALLELTYPEANALYSIS_H
#define PARALLELTYPEANALYSIS_H
/////////////////////////////////////////////////////////////////////
//  ParallelDepAnalysis.h - Parallel Analyzes file dependency      //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines a ParallelDepAnalysis class. It uses a 
filemgr to search certain directory and get files from it.
It create a tasks object, and using a threadpool to handle typeAnalysis
and depAnalysis process. 
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

class ParallelDepAnalysis
{
public:
	using CallObj = std::function<bool()>;
	ParallelDepAnalysis(std::string path, std::string filePatterns);
	void typeAnalysis();		
	void depAnalysis();
	void display();				//show result
	void mergeTable();			//table merge
	void combineResult();		//combine result from di_ and dio_
	void stop();
private:
	Tasks tasks_;
	FileManager::IFileMgr* fm_;
	TypeTable mergedTable_;				//table to store all type info
	BlockingQueue<TypeTable> tableQ_;	//table queue
	BlockingQueue<DependencyInfo> infoQ_;		//info queue
	std::vector<std::string> files_;	//stores all files' full path
	std::vector<DependencyInfo> results_;
	std::string path_;					//dir need to search
	std::string filePatterns_;			
	size_t fileCount = 0;				//file need to analyzed
	size_t fileProcessed = 0;			//file processed num
	size_t fileSearched = 0;			//file pass to threadpood
	std::mutex ioLock;
	std::future<size_t> futureTA_;		//future & promise use for typeAnalysis
	std::promise<size_t> promiseTA_;
	std::future<size_t> futureDA_;		//future & promise use for depAnalysis
	std::promise<size_t> promiseDA_;
};

#endif // !PARALLELTYPEANALYSIS_H

