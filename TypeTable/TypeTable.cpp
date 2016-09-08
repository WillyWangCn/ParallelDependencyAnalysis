#include "TypeTable.h"
/////////////////////////////////////////////////////////////////////
//  TypeTable.cpp - Records type information                       //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////


TypeRecord& TypeTable::operator[](size_t index)
{
	if(index<0 || index>=typeTable_.size())
		throw(std::exception("index out of range"));
	return typeTable_[index];
}

void TypeTable::addRecord(TypeRecord& record)
{
	typeTable_.push_back(record);
}

//merge two typeTables
std::vector<TypeRecord>& TypeTable::merge(TypeTable& table)
{
	std::vector<TypeRecord> types = table.getTypes();
	for (auto type : types)
		typeTable_.push_back(type);
	return typeTable_;
}

//print all records in a typeTable
void TypeTable::printTable()
{
	for (auto type : typeTable_)
		type.print();
}

#ifdef TEST_TYPETABLE
int main()
{
	std::cout << "\n ------ Test TypeTable ------";
	std::cout << "\n Create type records:";
	TypeRecord tr1("class", "Type1", "../test1.cpp");
	tr1.print();
	TypeRecord tr2("struct", "Type2", "../test1.cpp");
	tr2.print();
	TypeRecord tr3("struct", "Type3", "../test2.cpp");
	tr3.print();

	TypeTable tt1, tt2;
	std::cout << "\n\n Add to type table 1:";
	tt1.addRecord(tr1);
	tt1.addRecord(tr2);
	std::cout << "\n Print table1:";
	tt1.printTable();
	std::cout << "\n\n Add to type table 1:";
	tt2.addRecord(tr3);
	std::cout << "\n Print table2:";
	tt2.printTable();

	std::cout << "\n\n Merge to type table 1 & 2:";
	tt1.merge(tt2);
	std::cout << "\n Print table1:";
	tt1.printTable();

	std::cout << "\n";

}
#endif