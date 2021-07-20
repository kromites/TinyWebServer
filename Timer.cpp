#include "Timer.h"

USE_NAMESPACE

std::atomic<int64_t> s_numCreated_ = 0;

void Timer::restart(Timestamp now) {
	if(repeat_) {
		expiration_ = addTime(now, interval_);
	}else {
		expiration_ = Timestamp::invalid();
	}
}
