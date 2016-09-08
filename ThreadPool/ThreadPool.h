#ifndef THREADPOOL_H
#define THREADPOOL_H

/////////////////////////////////////////////////////////////////////
//  ThreadPool.h - A simple thread pool                            //
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
* This module defines simple threadpool. It refers Dr.Fawcett's design
* and implementation.
* Each thread deQs tasks from a blocking queue of callable objects,
* executes the callable object, and goes back to the queue for another.
* It stops when get a return false callobj.
*
* Required files
* Cpp11-BlockingQueue.h, Cpp11-BlockingQueue.cpp
*/


#include <thread>
#include <mutex>
#include <functional>
#include <vector>
#include <memory>
#include "../Logger/Cpp11-BlockingQueue.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"

class ThreadPool {
public:
	using CallObj = std::function<bool()>;
	ThreadPool(size_t num = 4) :threadNum_(num) {};
	~ThreadPool();
	void start();
	void doWork(CallObj& co) { coQ_.enQ(co); }   //public method to pass in callobj
	void wait();
private:
	BlockingQueue<CallObj> coQ_;
	std::vector<std::thread> threads_;
	size_t threadNum_;  //default as 4, so it has 4 threads
};

#endif // !THREADPOOL_H

