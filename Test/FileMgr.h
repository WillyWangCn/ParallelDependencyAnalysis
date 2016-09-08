#ifndef FILEMGR_H
#define FILEMGR_H

#include "IFileMgr.h"
#include <vector>
#include <iostream>
#include "../Logger/Cpp11-BlockingQueue.h"
#include "FileSystem.h"

namespace FileManager
{
	class FileMgr : public IFileMgr
	{
	public:
		FileMgr(std::string path, std::string patterns);
		std::string get();
		void search();
		void find(std::string path, std::string pattern);
		size_t fileCount() { return fileSearched_; }
	private:
		BlockingQueue<std::string> fileQ_;
		std::string path_;
		std::vector<std::string> patterns_;
		size_t fileSearched_ = 0;
	};
}
#endif
