#include "ThreadPool.h"

/////////////////////////////////////////////////////////////////////
//  ThreadPool.cpp - A simple thread pool                          //
//  ver 2.1                                                        //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
//  Sourse:        Prof.Jim Fawcett                                //
/////////////////////////////////////////////////////////////////////

//----< define threadProc and start threads >------------------------
void ThreadPool::start() {
	//keep deq callobjs
	std::function<void()> threadProc =
		[&]() {
		while (true)
		{
			CallObj co = coQ_.deQ();
			if (!co())
			{
				coQ_.enQ(co);
				break;
			}
		}
	};
	for (size_t i = 0; i < threadNum_; ++i)
	{
		//start threads to execute callobj
		std::thread t(threadProc);
		threads_.push_back(std::move(t));
	}
}
//join all threads
void ThreadPool::wait() {
	for (auto& thread : threads_)
		thread.join();
	coQ_.clear();
}
ThreadPool::~ThreadPool()
{
	for (auto& thread : threads_)
	{
		if (thread.joinable())
			thread.join();
	}
}