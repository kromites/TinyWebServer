#include "../base/Thread.h"
#include "../base/CurrentThread.h"


#include <list>
#include <cstdio>
#include <unistd.h>


std::list<int> buffer;
int cnt = 0;
MutexLock mutex_lock;


void threadFunc()
{
	printf("tid=%d\n", CurrentThread::tid());
}

void threadFunc2(int x)
{
	printf("tid=%d, x=%d\n", CurrentThread::tid(), x);
}

void threadFunc3()
{
	printf("tid=%d\n", CurrentThread::tid());
	sleep(1);
}

void product(int size) {
	for(int i=0; i<size; ++i) {
		{
			MutexLockGuard guard(mutex_lock);
			buffer.push_back(cnt);
			printf("generate num: %d\n", cnt++);
		}
		
	}
}


void consume() {
	while(true) {
		/* double check lock*/
		if (!buffer.empty()) {
			MutexLockGuard guard(mutex_lock);
			if (!buffer.empty()) {
				printf("consume num: %d, Thread id = %d\n", buffer.front(), CurrentThread::tid());
				buffer.pop_front();
			}
		}
	}
}



int main() {
	printf("pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
	Thread t1(threadFunc);
	t1.start();
	printf("t1.tid = %d\n", t1.getTid());
	printf("t1.name = %s\n", t1.name().c_str());
	printf("now thread numbers = %d\n", Thread::numCreated());

	t1.join();

	Thread t2(std::bind(threadFunc2, 42), "thread for free function with argument");
	t2.start();
	printf("t2.tid = %d\n", t1.getTid());
	printf("t2.name = %s\n", t2.name().c_str());
	printf("now thread numbers = %d\n", Thread::numCreated());
	t2.join();

	{
		Thread t6(threadFunc3);
		t6.start();
	}

	Thread t3(std::bind(product, 10000), "productor");
	Thread t4(consume, "consumer1");
	Thread t5(consume, "");
	
	t3.start();
	t4.start();
	t5.start();
	
	t3.join();
	t4.join();
	t5.join();

	
	printf("end of test\n");
	return 0;
}