#include "FileMgr.h"

using namespace FileManager;

FileMgr::FileMgr(std::string path, std::string patterns) : path_(path)
{
	size_t index = 0;
	size_t found = patterns.find_first_of(',', index);
	while (found != std::string::npos)
	{
		patterns_.push_back(patterns.substr(index, found - index));
		index = found + 1;
		found = patterns.find_first_of(',', index);
	}
	if (found - index > 0)
	{
		patterns_.push_back(patterns.substr(index, found - index));
	}
}

std::string FileMgr::get()
{
	return fileQ_.deQ();
}

void FileMgr::find(std::string path, std::string pattern) {
	std::vector<std::string> files = FileSystem::Directory::getFiles(path, pattern);
	for (auto f : files)
	{
		fileQ_.enQ(path+"\\"+f);
		fileSearched_ ++;
		//std::cout << "\n find file in path: " << f;
	}
	std::vector<std::string> dirs = FileSystem::Directory::getDirectories(path);
	//get current directories
	for (auto d : dirs)
	{
		if (d == "." || d == "..")
			continue;
		std::string dpath = path + "\\" + d;    //go to deeper directory
		find(dpath, pattern);
	}
}

void FileMgr::search()
{
	std::string fpath = FileSystem::Path::getFullFileSpec(path_);
	for (auto pattern : patterns_) 
	{
		find(fpath, pattern);
	}
	//fileQ_.enQ("Search Finished!");
}

#ifdef TEST_FILEMGR
int main() 
{
	std::cout << "\n ----------- Test FileMgr -----------";
	std::string patterns = "*.h,*.cpp";
	std::string path = "..\\Test";
	std::cout << "\n Find " << patterns << " in " << path<<std::endl;
	IFileMgr* filemgr = FileMgrFactory::create(path, patterns);
	filemgr->search();
	std::cout << "\n " << filemgr->fileCount()<<" file searched!";
	while (true)
	{
		std::string result = filemgr->get();
		std::cout << "\n find file: " << result;
	}
}
#endif