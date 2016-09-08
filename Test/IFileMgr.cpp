#include "IFileMgr.h"
#include "FileMgr.h"

using namespace FileManager;

IFileMgr* FileMgrFactory::create(const std::string& path, const std::string& patterns) {
	return new FileMgr(path, patterns);
}