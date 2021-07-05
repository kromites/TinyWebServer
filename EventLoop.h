#pragma once

#include <any>
#include <vector>
#include <memory>

#include "./base/Util.h"
#include "./base/Timestamp.h"
#include "Channel.h"
#include "./base/Mutex.h"

START_NAMESPACE

class Poller;
class Channel;

class EventLoop {
public:
	using Functor = std::function<void()>;
	using TimeCallback = std::function<void()>;
	
	EventLoop();
	~EventLoop();

	void loop();

	void assertInLoopThread() const;

	bool isInLoopThread() const;

	void quit();

	void wakeup() const; // write, to make this Loop awake.
	void handleRead() const; // read
	// for poller to manage channel

	void insertChannel(Channel& channel) const;
	void updateChannel(Channel& channel) const;
	void removeChannel(Channel& channel) const;
	bool hasChannel(Channel& channel) const;

	// for TimeQueue
	Timestamp runAt(const Timestamp& time, const TimeCallback& cb);

	Timestamp runAfter(double delay, const TimeCallback& cb);

	Timestamp runEvery(double interval, const TimeCallback& cb);

	// context

	void setContext(const std::any& context)
	{
		context_ = context;
	}

	const std::any& getContext() const
	{
		return context_;
	}

	std::any* getMutableContext()
	{
		return &context_;
	}

	// cancels the timer
	void cancel(Timestamp timerId);

	//
	void runInLoop(const Functor& callback);
	void queueInLoop(const Functor& callback);
	void doPendingFunctors();

private:
	void abortNotInLoopThread() const;

	typedef std::vector<Channel*> ChannelList;

	static int KEpollTimeMs;
	
	bool looping_;  /* atomic */
	bool quit_;  /* atomic */
	bool callingFunctorQueue_;
	bool handleEvent_;
	const pid_t threadId_;
	Timestamp pollReturnTime_;
	std::unique_ptr<Poller> poller_;
	// std::unique_ptr<TimerQueue> timerQueue_;


	// wakeup epoll_wait
	int wakeupFd_;
	Channel wakeupChannel_;
	ChannelList activeChannels_;

	Channel* curActiveChannel_;
	
	MutexLock mutex_;
	
	// exposed to other threads
	std::vector<Functor> FunctorQueue_;
	std::any context_;
};


END_NAMESPACE