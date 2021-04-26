#include "EventLoop.h"

#include "unistd.h"
#include <cassert>

#include "./base/CurrentThread.h"
#include "./base/Logger.h"
#include "Channel.h"
#include "Poller.h"

__thread EventLoop* t_loopInThisThread = 0;

int EventLoop::KEpollTimeMs = -1;

EventLoop::EventLoop() :looping_(false), threadId_(CurrentThread::tid()), poller_(new Poller()) {
	LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;

	if(t_loopInThisThread) {
		LOG_FATAL << "Another EventLoop " << t_loopInThisThread << " exits in this thread " << threadId_;
	}else {
		t_loopInThisThread = this;
	}
}

EventLoop::~EventLoop() {
	assert(!looping_);
	t_loopInThisThread = nullptr;
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
		for(auto it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
			(*it)->handleEvent();
		}
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


EventLoop* EventLoop::getEventLoopOfCurrentThread() {
	return t_loopInThisThread;
}

void EventLoop::quit() {
	quit_ = true;
	// wakeup();
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

void EventLoop::abortNotInLoopThread() const {
	LOG_FATAL << "This EventLoop shoule be in thread " << threadId_ << " but current in thread " << CurrentThread::tid();
}



