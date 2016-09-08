#ifndef FILEMGR_H
#define FILEMGR_H

/////////////////////////////////////////////////////////////////////
//  FileMgr.h - Search files from certain path                     //
//  ver 2.1                                                        //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr2                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines the operation of finding files from certain direcoty.
It returns a vector of the file's names and path.
It implements the interfaces define by IFileMgr.

Required files
FileSystem.h, FileSystem.cpp
*/

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
