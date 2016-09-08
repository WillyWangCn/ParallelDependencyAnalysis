#include "IFileMgr.h"
#include "FileMgr.h"
/////////////////////////////////////////////////////////////////////
//  IFileMgr.cpp - Interface and factory for fileMg                //
//  ver 2.1                                                        //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////

using namespace FileManager;

IFileMgr* FileMgrFactory::create(const std::string& path, const std::string& patterns) {
	return new FileMgr(path, patterns);
}