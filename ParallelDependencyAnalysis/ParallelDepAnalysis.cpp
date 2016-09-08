#include <iostream>
#include <string>
#include <thread>
#include "ParallelDepAnalysis.h"
/////////////////////////////////////////////////////////////////////
//  ParallelDepAnalysis.cpp - Parallel Analyzes file dependency    //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////
ParallelDepAnalysis::ParallelDepAnalysis(std::string path, std::string filePatterns)
	: path_(path), filePatterns_(filePatterns)
{
	fm_ = FileManager::FileMgrFactory::create(path, filePatterns);
}
//////////////////////////////////////////////////////////////////
// this method call filemgr to search file and creates typeAnalysis
// callobj for tasks.
void ParallelDepAnalysis::typeAnalysis()
{
	//bind future to promise
	futureTA_ = promiseTA_.get_future();
	std::cout << "\n\n Starting search files " << "in \"" << path_ << "\"\n";
	fm_->search();
	fileCount = fm_->fileCount();
	while (fileCount != fileSearched) {
		std::string path = fm_->get();
		files_.push_back(path);			//store full path for further use
		TypeTable tempTable;
		//for each file, do type analysis in a callobj
		Tasks::CallObj cota = [=]()-> bool {
			{
				std::lock_guard<std::mutex> l(ioLock);
				std::cout << "\n Analyzing types in " << path << " ... ";
			}
			ConfigParseToConsole configure;
			TypeAnalysis* ta_ = configure.Build();
			if (ta_)
			{
				if (!configure.Attach(path))
				{
					std::cout << "\n  Could not open file " << path << std::endl;
					return true;
				}
			}
			else
			{
				std::cout << "\n\n TypeAnalysis not built\n\n";
				return true;
			}
			while (ta_->next())			//keep get semi and analyze it
				ta_->analyze();
			tableQ_.enQ(configure.getTable());		//enQ typeinfo
			fileProcessed++;
			//if all file processed, set value for promise
			if (fileProcessed == fileCount)
				promiseTA_.set_value(fileProcessed);
			return true;
		};
		tasks_.run(cota);			//pass to threadpool
		fileSearched++;
	}
	mergeTable();
}

//////////////////////////////////////////////////////////////////
// this creates depAnalysis callobj for tasks.
// also let main thread to show result
void ParallelDepAnalysis::depAnalysis()
{
	fileProcessed = 0;
	futureDA_ = promiseDA_.get_future();
	std::cout << "\n\n Starting analyzing dependency... ";
	std::cout << "\n ================================== ";
	for (auto file : files_)
	{
		Tasks::CallObj coda = [=]()-> bool {
			{
				std::lock_guard<std::mutex> l(ioLock);
				std::cout << "\n Analyzing dependency in " << file << " ... ";
			}
			DependencyAnalysis da(mergedTable_);
			if (!da.attach(file))
			{
				std::cout << "\n  could not open file " << file << std::endl;
				return true;
			}
			da.analyze();
			da.removeDuplicate();
			infoQ_.enQ(da.getDepInfo());		//enQ di_ and dio_
			infoQ_.enQ(da.getDepedInfo());
			fileProcessed++;
			//if all file processed, set value for promise
			if (fileProcessed == fileCount)
				promiseDA_.set_value(fileProcessed);
			return true;
		};
		tasks_.run(coda);
	}
	display();
}

//mergeTable and show typeAnalysis result
void ParallelDepAnalysis::mergeTable()
{
	//will wait till all file processed
	size_t fileP = futureTA_.get();
	std::cout << "\n\n ================================== ";
	std::cout << "\n Type analysis finished. " << fileCount << " files analyzed!";;
	std::cout << "\n Analysis result:";
	size_t searchNum = 0;
	while (searchNum != fileCount)
	{
		TypeTable tt = tableQ_.deQ();
		mergedTable_.merge(tt);
		searchNum++;
	}
	mergedTable_.printTable();
	std::cout << "\n";
}
//combine result from di_ and dio_ and show depAnalysis result
void ParallelDepAnalysis::display()
{
	//will wait till all file processed
	size_t fileP = futureDA_.get();
	std::cout << "\n\n ================================== ";
	std::cout << "\n Dependency analysis finished. " << fileCount << " files analyzed!";;
	std::cout << "\n Analysis result:";
	size_t searchNum = 0;
	while (searchNum != (fileCount*2))
	{
		DependencyInfo di = infoQ_.deQ();
		results_.push_back(di);
		searchNum++;
	}
	combineResult();
	for (auto result : results_)
		result.showDep();
}

void ParallelDepAnalysis::combineResult()
{
	//erase empty records
	for (std::vector<DependencyInfo>::iterator it = results_.begin(); it != results_.end();)
	{
		if (it->getName() == " ")
			it = results_.erase(it);
		else
			it++;
	}
	//for each file, combines all dependency info into one record  
	for (std::vector<DependencyInfo>::iterator it = results_.begin(); it != results_.end();)
	{
		for (std::vector<DependencyInfo>::iterator it2 = it+1; it2 != results_.end();)
		{
			if (it->getName() == it2->getName())
			{
				for (auto r : it2->getDepFiles())
					it->getDepFiles().push_back(r);
				it->removeDuplicate();
				it2 = results_.erase(it2);
			}
			else
				it2++;
		}
		it++;
	}
}
//send signal for threadpoop to stop
void ParallelDepAnalysis::stop()
{
	CallObj exit = []() -> bool { return false; };
	tasks_.run(exit);
}

#ifdef TEST_PDA
int main()
{
	std::cout << "\n ----------- Test ParallelDepAnalysis -----------";
	std::string filePatterns = "*.h,*.cpp";
	std::string path = "../Test";
	ParallelDepAnalysis pda(path, filePatterns);
	pda.typeAnalysis();
	pda.depAnalysis();
	pda.stop();
	std::cout << "\n\n";
}
#endif