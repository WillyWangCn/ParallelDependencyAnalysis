#ifndef IFILEMGR_H
#define IFILEMGR_H

#include <string>
#include <iostream>

namespace FileManager {
	struct IFileMgr {
	public:
		virtual std::string get() = 0;
		virtual void search() = 0;
		virtual size_t fileCount() = 0;
	};
	struct FileMgrFactory
	{
		static IFileMgr* create(const std::string& path, const std::string& pattern);
	};
}
#endif