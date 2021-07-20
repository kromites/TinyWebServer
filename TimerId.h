#pragma once
#include <cstdint>

#include "base/Util.h"

START_NAMESPACE

class Timer;

// the Timer id, for canceling Timer.
class TimerId {
public:
	TimerId() : timer_(nullptr), seq_(0) {}
	TimerId(Timer* timer, int64_t seq) : timer_(timer), seq_(seq) {}

	friend class TimerQueue;
private:
	Timer* timer_;
	int64_t seq_;
};

END_NAMESPACE