/////////////////////////////////////////////////////////////////////
//  TypeAnalysis.cpp - Analyzes C++ language dependency            //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////

#include <fstream>
#include <algorithm> 
#include "../TypeTable/TypeTable.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "DependencyAnalysis.h"

using namespace Scanner;


void DependencyInfo::setName(std::string name)
{
	size_t index = name.find_last_of('\\');
	std::string fileName = name.substr(index + 1);
	fileName_ = fileName;
}

void DependencyInfo::showDep() {
	std::cout << "\n  " << fileName_ << " depends on:";
	for (auto file : depFiles_)
		std::cout << "\n   -- " << file;
}
//remove duplicate keys from the vector
void DependencyInfo::removeDuplicate() {
	//first sort, then call unique method to remove duplicates
	sort(depFiles_.begin(), depFiles_.end());

	depFiles_.erase(unique(depFiles_.begin(), depFiles_.end()), depFiles_.end());
	for (std::vector<std::string>::iterator iter = depFiles_.begin(); iter != depFiles_.end();)
	{
		if (*iter == fileName_)
			iter = depFiles_.erase(iter);
		else
			iter++;
	}
}

//constructor & destructor
DependencyAnalysis::DependencyAnalysis(TypeTable& table) :table_(table)
{
	pToker = new Toker;
	pToker->returnComments(false);
	pSemi = new SemiExp(pToker);
}

DependencyAnalysis::~DependencyAnalysis()
{
	delete pSemi;
	delete pToker;
	pIn->close();
	delete pIn;
}

//----< attach toker to a file stream or stringstream >------------
bool DependencyAnalysis::attach(std::string file)
{
	if (pToker == 0)
		return false;
	pIn = new std::ifstream(file);
	if (!pIn->good())
		return false;
	di_.setName(file);
	return pToker->attach(pIn);
}

bool DependencyAnalysis::next()
{
	//if can get next semi?
	bool succeeded = pSemi->get();
	if (!succeeded)
		return false;
	return true;
}

//analyze dependency
void DependencyAnalysis::analyze()
{
	//for each semi, do checkdependency operation
	while (pSemi->get())
	{
		ITokCollection& tc = *pSemi;
		checkDependency(tc);
	}
}

void DependencyAnalysis::checkDependency(ITokCollection& tc)
{
	//first get all the include files and stored in includes_ for futher use
	if (tc[0] == "#" && tc[1] == "include" && tc[2][0] == '"')
	{
		//get file's full path
		std::string file = tc[2].substr(1, tc[2].length() - 2);
		//substring it to get the filename
		std::string fileName;
		size_t index = file.find_last_of('\\');
		if (index != std::string::npos)
			fileName = file.substr(index + 1);
		else
		{
			index = file.find_last_of('/');
			fileName = file.substr(index + 1);
		}
		includes_.push_back(fileName);
	}
	//dependency check operations:
	// 1> if is using namespace XXX case
	if ((tc.find("using") < tc.length()) && tc.find("namespace") < tc.length())
	{
		//go through the typeTable to find this namespace
		for (auto record : table_.getTypes())
		{
			//find the namespace and add to records
			if (record.getType() == "namespace" && record.getName() == tc[tc.find("namespace") + 1])
				di_.addDepFile(record.getFile());
		}
	}
	// 2> C::fun() {} case
	// in this case, we need to find out which class/namespace defines this fun
	else if (tc.find("::") < tc.length() && tc[tc.find("::") + 2] == "(" && tc[tc.length() - 1] == "{")
	{
		std::string type = tc[tc.find("::") - 1];		//get the name of this class/namespace

		for (auto record : table_.getTypes())
		{
			std::string file = record.getFile();
			for (auto f : includes_)
			{
				if (f == file && record.getName() == type)
				{
					dio_.setName(record.getFile());
					dio_.addDepFile(di_.getName());
				}
			}
		}
	}
	// 3> C c;
	// in this case, we need to find in which file, C is defined
	else if (tc[0] != "#")
	{
		for (auto record : table_.getTypes())
		{
			std::string type = record.getName();
			std::string file = record.getFile();
			//if semi contains a type name
			if (tc.find(type) < tc.length())
			{
				//because we use this type, so for sure we includes the head file of this type
				for (auto f : includes_)
					if (f == file)
						di_.addDepFile(file);
			}
		}
	}

}

void DependencyAnalysis::printDepInfo()
{
	di_.showDep();
}
//remove duplicates
void DependencyAnalysis::removeDuplicate()
{
	di_.removeDuplicate();
	dio_.removeDuplicate();
}

#ifdef TEST_DA 
int main()
{
	std::cout << "\n ------ Test DependencyAnalysis ------";
	std::cout << "\n Create type records:";
	TypeRecord tr1("class", "search", "test1.cpp");
	tr1.print();
	TypeRecord tr2("struct", "get", "test1.cpp");
	tr2.print();
	TypeRecord tr3("struct", "Type3", "test2.cpp");
	tr3.print();

	TypeTable tt;
	tt.addRecord(tr1);
	tt.addRecord(tr2);
	tt.addRecord(tr3);
	std::cout << "\n\n Analysis result:";
	DependencyAnalysis da(tt);
	da.attach("..\\Test\\Grandparent.h");
	da.analyze();
	da.removeDuplicate();
	DependencyInfo di = da.getDepInfo();
	di.showDep();
	std::cout << "\n\n";
}
#endif // TEST_DA 
