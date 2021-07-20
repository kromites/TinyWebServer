#pragma once

#include <functional>
#include <atomic>

#include "base/Util.h"
#include "base/noncopyable.h"
#include "base/Timestamp.h"
START_NAMESPACE

//  class for timer event -> bind with the timer callback.

class Timer:noncopyable {
private:
	using TimerCallback = std::function<void()>;
public:
	Timer(TimerCallback cb, Timestamp when, double interval)
		:timercallback_(std::move(cb)),
		expiration_(when),
		interval_(interval),
		repeat_(interval > 0.0),
		seq_(s_numCreated_+1) {}
	void run() const {
		timercallback_();
	}
	
	Timestamp expiration() const { return expiration_; }

	bool repeat() const { return repeat_; }
	int64_t seq() const { return seq_; }
	
	void restart(Timestamp now);
	
	static int64_t numCreated() { return s_numCreated_; }


private:
	TimerCallback timercallback_;
	Timestamp expiration_;
	const double interval_;
	const bool repeat_;
	const int64_t seq_;
	
	static std::atomic<int64_t> s_numCreated_;
};

END_NAMESPACE