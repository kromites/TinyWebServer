#pragma once
#include <memory>
#include <vector>

#include "base/Util.h"
#include "EventLoopThread.h"

START_NAMESPACE

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool {
public:
	using ThreadInitCallback = std::function<void(EventLoop*)>;

	// default param must be the tail of param list.
	EventLoopThreadPool(const ThreadInitCallback& func, size_t size = 4, const std::string& name = "");
	~EventLoopThreadPool();

	void setThreadNum(int numThreads) { numThreads_ = numThreads; }
	void start();
	EventLoop* getNextLoop();
	const std::string threadName() const;

private:
	size_t curIncrease();
	bool started_;
	const size_t loopSize_;
	std::string name_;
	EventLoop* baseLoop_;
	
	int numThreads_;
	int next_;           // always in loop thread;
	
	std::vector<EventLoop*> loops_;
	std::vector<std::unique_ptr<EventLoopThread>> threads_;
};

END_NAMESPACE