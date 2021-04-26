#include <unistd.h>

#include "../EventLoop.h"
#include "../base/CurrentThread.h"
#include "../base/Thread.h"
#include "../base/Logger.h"

void threadFunc() {
	LOG_DEBUG << "ThreadFunc(): pid = " << getpid() << " tid = " << CurrentThread::tid();
	EventLoop loop;
	loop.loop();
	
}

int main() {
	LOG_DEBUG << "ThreadFunc(): pid = " << getpid() << " tid = " << CurrentThread::tid();
	EventLoop loop;

	Thread thread(threadFunc);
	thread.start();

	loop.loop();

	thread.join();
	sleep(60);
	return 0;
}