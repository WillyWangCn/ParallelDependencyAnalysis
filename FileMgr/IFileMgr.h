#ifndef IFILEMGR_H
#define IFILEMGR_H

/////////////////////////////////////////////////////////////////////
//  IFileMgr.h - Interface and factory for fileMg                  //
//  ver 2.1                                                        //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines the interface and factory of filemgr.

*/

#include <string>
#include <iostream>

namespace FileManager {
	struct IFileMgr {
	public:
		virtual std::string get() = 0;
		virtual void search() = 0;
		virtual size_t fileCount() = 0;
	};
	//filemgr factory
	struct FileMgrFactory
	{
		static IFileMgr* create(const std::string& path, const std::string& pattern);
	};
}
#endif