#pragma once
#include <functional>
#include <set>
#include <vector>


#include "base/Util.h"
#include "base/noncopyable.h"
#include "base/Timestamp.h"
#include "Channel.h"

START_NAMESPACE



class EventLoop;
class Timer;
class TimeId;


class TimerQueue:noncopyable {
private:
	using TimerCallback = std::function<void()>;
	using Entry = std::pair<Timestamp, Timer*>;
	using TimerList = std::set<Entry>;
	using ActiveTimer = std::pair<Timer*, int64_t>;
	using ActiveTimerSet = std::set<ActiveTimer>;

public:
	explicit TimerQueue(EventLoop* loop);
	~TimerQueue();
	TimeId addTimer(TimerCallback cb, Timestamp when, double interval);

	void cancel(TimeId timeid);

private:
	// call back function for add and cancel
	void addTimerInloop(Timer* timer);
	void cancleInloop(TimeId timeId);
	// called when timefd alarms
	void handleRead();
	// move out all expired times
	std::vector<Entry> getExipired(Timestamp now);
	void reset(Timestamp now);
	void reset(const std::vector<Entry>& expired, Timestamp now);

	bool insert(Timer* timer);
	
	
	EventLoop* loop_;
	const int timerfd_;
	Channel timerfdChannel_;

	// sorted by expiration
	TimerList timers_;

	// for cancel(

	ActiveTimerSet activeTimers_;
	bool callingExpiredTimers; // atomic
	ActiveTimerSet cancelingTimers_;
};



END_NAMESPACE