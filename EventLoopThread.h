#pragma once

#include "./base/Util.h"
#include "./base/Thread.h"

START_NAMESPACE

class EventLoop;

// one loop one thread

class EventLoopThread {
public:
	typedef std::function<void(EventLoop*)> ThreadInitCallback;
	
	EventLoopThread(const std::string& name = "",const ThreadInitCallback& cb = ThreadInitCallback());
	~EventLoopThread();
	EventLoop* startLoop();
	
private:
	void threadFunc();
	
	Thread thread_;
	MutexLock mutex_;
	Condition cond_;
	EventLoop* loop_;
	ThreadInitCallback callback_;
};

END_NAMESPACE