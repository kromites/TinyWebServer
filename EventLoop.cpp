#include "EventLoop.h"

#include <memory>
#include "unistd.h"
#include <cassert>

#include "./base/CurrentThread.h"
#include "./base/Logger.h"
#include "Socket.h"
#include "Channel.h"
#include "Poller.h"

USE_NAMESPACE


int EventLoop::KEpollTimeMs = -1;

EventLoop::EventLoop() :looping_(false),
						quit_(false),
						callingFunctorQueue_(false),
						handleEvent_(false),
						threadId_(CurrentThread::tid()),
						poller_(new Poller()),
						wakeupFd_(eventFd()),
						wakeupChannel_(this, wakeupFd_),
						curActiveChannel_(nullptr)
{
	LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;

	wakeupChannel_.setReadCallback([this]() { handleRead(); });
	wakeupChannel_.enableReading();
}

EventLoop::~EventLoop() {
	wakeupChannel_.disableReadAndWrite();
	Close(wakeupFd_);
}

// core function in EventLoop
void EventLoop::loop() {
	LOG_TRACE << "start loop in EventLoop " << this;
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;
	quit_ = false;

	// call the Channel::handleEvent()
	while(!quit_) {
		activeChannels_.clear();
		poller_->poll(activeChannels_, KEpollTimeMs);
		handleEvent_ = true;
		for(auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
			(*it)->handleEvent();
		}

		handleEvent_ = false;
		
		doPendingFunctors();
	}
	LOG_TRACE << "EventLoop " << this << " stop looping";
	looping_ = false;
}

void EventLoop::assertInLoopThread() const{
	if (!isInLoopThread()) {
		abortNotInLoopThread();
	}
}

bool EventLoop::isInLoopThread() const {
	return threadId_ == CurrentThread::tid();
}


void EventLoop::quit() {
	quit_ = true;
	if(!isInLoopThread()) {
		wakeup();
	}
}

void EventLoop::wakeup() const {
	// just write ont byte to wake up this loop
	uint64_t one = 1;
	ssize_t n = ::write(wakeupFd_, &one, sizeof one);
	if(n != sizeof one) {
		LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
	}
}

void EventLoop::handleRead() const {
	// ET
	uint64_t val = 1;
	while (true) {
		const auto ret = ::read(wakeupFd_, &val, sizeof val);
		if (ret < 0) {
			if (errno == EAGAIN)
				break;
			else if (errno == EINTR)
				continue;
			else {
				LOG_FATAL << "EventLoop::handleWakeUp() error";
			}
		}
		else if (ret != sizeof val) {
			LOG_ERROR << "EventLoop::handleWakeUp() reads " << ret << " bytes instead of 8";
		}
	}
}

void EventLoop::insertChannel(Channel& channel) const {
	assert(channel.ownerLoop() == this);
	assertInLoopThread();
	poller_->insert(channel);
}

void EventLoop::updateChannel(Channel& channel) const {
	assert(channel.ownerLoop() == this);
	assertInLoopThread();
	poller_->update(channel);
}

void EventLoop::removeChannel(Channel& channel) const {
	assert(channel.ownerLoop() == this);
	assertInLoopThread();
	poller_->remove(channel);
}

bool EventLoop::hasChannel(Channel& channel) const {
	assert(channel.ownerLoop() == this);
	assertInLoopThread();
	return poller_->hasChannel(channel);
}

void EventLoop::runInLoop(const Functor& callback) {
	if(isInLoopThread()) {
		callback();
	}else {
		queueInLoop(callback);
	}
}

void EventLoop::queueInLoop(const Functor& callback) {
	// need lock
	{
		MutexLockGuard guard(mutex_);
		FunctorQueue_.push_back(callback);
	}
	if(!isInLoopThread() || handleEvent_) {
		// wake up this event loop
		wakeup();
	}
}

void EventLoop::doPendingFunctors() {
	std::vector<Functor> functors;
	callingFunctorQueue_ = true;
	{
		MutexLockGuard guard(mutex_);
		functors.swap(FunctorQueue_);
	}
	for(size_t i = 0; i<functors.size(); ++i) {
		functors[i]();
	}
	callingFunctorQueue_ = false;
}

void EventLoop::abortNotInLoopThread() const {
	LOG_FATAL << "This EventLoop should be in thread " << threadId_ << " but current in thread " << CurrentThread::tid();
}



