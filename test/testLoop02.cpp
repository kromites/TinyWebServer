#include <unistd.h>

#include "../EventLoop.h"
#include "../base/CurrentThread.h"
#include "../base/Thread.h"
#include "../base/Logger.h"

EventLoop* g_loop;

void threadFunc() {
	g_loop->loop();

}

int main() {
	LOG_DEBUG << "ThreadFunc(): pid = " << getpid() << " tid = " << CurrentThread::tid();
	EventLoop loop;
	g_loop = &loop;
	Thread thread(threadFunc);
	thread.start();

	thread.join();
	sleep(60);
	return 0;
}