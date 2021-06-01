#include "EventLoopThread.h"
#include "EventLoop.h"

#include <cassert>

EventLoopThread::EventLoopThread( const std::string& name, const ThreadInitCallback& cb) :
	thread_([this]() {this->threadFunc(); }, name),
	mutex_(),
	cond_(mutex_),
	loop_(nullptr),
	callback_(cb)
{
}

EventLoopThread::~EventLoopThread() {
	if(loop_) {
		loop_->quit();
		thread_.join();
	}
}

EventLoop* EventLoopThread::startLoop() {
	assert(!thread_.isStarted());
	thread_.start();
	{
		MutexLockGuard guard(mutex_);
		while (loop_ == nullptr) {
			cond_.wait(); 
		}
	}
	return loop_;
}

void EventLoopThread::threadFunc() {
	EventLoop loop;
	if(callback_) {
		callback_(&loop);
	}
	
	{
		MutexLockGuard guard(mutex_);
		loop_ = &loop;
		cond_.notify();
	}
	
	loop.loop();

	MutexLockGuard lock(mutex_);
	loop_ = nullptr;
}
