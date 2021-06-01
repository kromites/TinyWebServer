#include "EventLoopThreadPool.h"

#include "EventLoop.h"
#include "EventLoopThread.h"


EventLoopThreadPool::EventLoopThreadPool(const ThreadInitCallback& func, size_t size, const std::string& name ):
	started_(false), loopSize_(size), name_(name)
{
	for(auto i=0; i<loopSize_; ++i) {
		const std::string threadName = name + std::to_string(i);
		EventLoopThread* t = new EventLoopThread(threadName, func);
		threads_.push_back(std::unique_ptr<EventLoopThread>(t));
	}
}

EventLoopThreadPool::~EventLoopThreadPool() {
	
}

void EventLoopThreadPool::start() {
	started_ = true;
	for (auto& thread : threads_) {
		loops_.push_back(thread->startLoop());
	}
}

EventLoop* EventLoopThreadPool::getNextLoop() {
	return loops_[curIncrease()];
}

const std::string EventLoopThreadPool::threadName() const{
	return name_;
}

size_t EventLoopThreadPool::curIncrease() {
	if(++next_ == loopSize_) {
		next_ = 0;
	}
	return next_;
}



