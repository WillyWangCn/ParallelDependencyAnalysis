#ifndef TASKS_H
#define TASKS_H

/////////////////////////////////////////////////////////////////////
//  Tasks.h - A package uses threadpool                            //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
//  Sourse:        Prof.Jim Fawcett                                //
/////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module runs the threadpool and keep enQ callobjs. 
* It refers Dr.Fawcett's design and implementation.
*
* Required files
* Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
* ThreadPool.h, ThreadPool.cpp
*/

#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <memory>
#include "../ThreadPool/ThreadPool.h"

class Tasks
{
public:
	using CallObj = std::function<bool()>;
	Tasks();
	void run(CallObj co);
	void wait();
private:
	ThreadPool tp_;
};
#endif // !TASKS_H

