#include "TimerQueue.h"

#include <unistd.h>


#include "base/Logger.h"
#include "EventLoop.h"
#include "Timer.h"
#include "TimerId.h"

#include <sys/timerfd.h>

USE_NAMESPACE

int createTimerfd() {
	// create timer fd
	auto timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
	if(timerfd < 0) {
		LOG_FATAL << "Failed in timerfd_create";
	}

	return timerfd;
}

timespec howMuchTimeFromNow(Timestamp when) {
	auto microseconds = when.microSecondsSinceEpoch() - Timestamp::now().microSecondsSinceEpoch();
	if(microseconds < 100) {
		microseconds = 100;// why?
	}
	timespec ts;
	ts.tv_sec = static_cast<time_t>(microseconds / kMicroSecondsPerSecond);
	ts.tv_nsec =static_cast<long>(microseconds % kMicroSecondsPerSecond * 1000);
	return ts;
}


TimerQueue::TimerQueue(EventLoop* loop)
	: loop_(loop), timerfd_(createTimerfd()), timerfdChannel_(loop, timerfd_),
		timers_(), callingExpiredTimers(false)
{
	timerfdChannel_.setReadCallback([this]() {
		TimerQueue::handleRead();
		});
	timerfdChannel_.enableReading();
}

TimerQueue::~TimerQueue() {
	timerfdChannel_.disableReadAndWrite();
	timerfdChannel_.remove();
	::close(timerfd_);

	for(const auto& timer : timers_) {
		
	}
}




