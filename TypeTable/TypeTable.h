#ifndef TYPETABLE_H
#define TYPETABLE_H
/////////////////////////////////////////////////////////////////////
//  TypeTable.h - Records type information                         //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
/////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module defines the TypeRecord and TypeTable.
* TypeRecord is used to store type informations, and TypeTable is 
* used to store TypeRecords.
* TypeRecord contians the type, the name and in which file it defineds.
*
* Required files
* Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
* ThreadPool.h, ThreadPool.cpp
*/
#include <iostream>
#include <vector>
#include <string>

class TypeRecord
{
public:
	TypeRecord(std::string type, std::string name, std::string file ):type_(type),file_(file),name_(name) {}
	std::string& getName() { return name_; }
	std::string& getFile() { return file_; }
	std::string& getType() { return type_; }
	void print() { std::cout << "\n  Type:" << type_ << ",   name:" << name_ << ",   file:" << file_; }
private:
	std::string type_;
	std::string file_;
	std::string name_;
};

class TypeTable
{
public:
	TypeRecord& operator[](size_t index);   //operator []
	void addRecord(TypeRecord& record);		//add a record by passing a typerecord
	std::vector<TypeRecord>& getTypes() { return typeTable_; }		//return the type table
	std::vector<TypeRecord>& merge(TypeTable& table);		//merge two type tables
	void printTable();
private:
	std::vector<TypeRecord> typeTable_;		//a vector used to store typeRecords
	std::vector<TypeRecord>::iterator it_;
};

#endif // !TYPETABLE_H

