#include <unistd.h>

#include "../EventLoop.h"
#include "../Channel.h"
#include "../Poller.h"
#include "../base/Thread.h"

#include <sys/timerfd.h>

EventLoop* g_loop;

void timeout() {
	printf("Timeout!\n");
	g_loop->quit();
}

int main() {
	EventLoop loop;
	g_loop = &loop;

	int timefd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	Channel channel(&loop, timefd);
	channel.setReadCallback(timeout);
	channel.enableReading();

	
	
	struct itimerspec howlong;
	bzero(&howlong, sizeof howlong);
	howlong.it_value.tv_sec = 5;
	::timerfd_settime(timefd, 0, &howlong, nullptr);

	LOG_TRACE << "start loop";
	
	loop.loop();

	::close(timefd);
	LOG_TRACE << "end clock";
}