#include "../Tasks/Tasks.h"
#include <string>
#include <iostream>

/////////////////////////////////////////////////////////////////////
//  Tasks.cpp - A package uses threadpool                          //
//                                                                 //
//  Language:      Visual C++                                      //
//  Application:   CSE687 Pr3                                      //
//  Author:        Yingxuan Wang                                   //
//                 ywang206@syr.edu                                //
//  Sourse:        Prof.Jim Fawcett                                //
/////////////////////////////////////////////////////////////////////

//start threadpool
Tasks::Tasks() {
	tp_.start();
}
//enq callobj
void Tasks::run(CallObj co) {
	tp_.doWork(co);
}

void Tasks::wait() {
	tp_.wait();
}

#ifdef TEST_TASKS

int main()
{
	std::cout << "\n  main thread id = " << std::this_thread::get_id();
	Tasks t;
	Tasks::CallObj co = []()->bool {
		std::cout << "\n  Hello from working thread " << std::this_thread::get_id();
		return true;
	};
	for (size_t i = 0; i < 20; ++i)
	{
		t.run(co);
	}
	Tasks::CallObj exit = []() ->bool { return false; };
	t.run(exit);
}
#endif