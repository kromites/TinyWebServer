#include "Channel.h"
#include "base/Logger.h"

#include <cassert>
#include <sys/epoll.h>
#include "EventLoop.h"
#include "Poller.h"

USE_NAMESPACE

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = EPOLLIN | EPOLLPRI;
const int Channel::kWriteEvent = EPOLLOUT;


Channel::Channel(EventLoop* loop, int fd): loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1), eventHandling_(false)
{
}

Channel::~Channel() {
	assert(!eventHandling_);
}

// channel core code : call by the Eventloop::loop()
void Channel::handleEvent() {
	
	LOG_TRACE << eventToString(fd_, revents_);
	eventHandling_ = true;
	
	if((revents_ & EPOLLHUP) && !(revents_ & EPOLLIN)) {
		if (closeCallback_)
			closeCallback_();
	}

	if(revents_ & EPOLLERR) {
		if (errorCallback_)
			errorCallback_();
	}

	if(revents_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP)) {
		if (readCallback_)
			readCallback_();
	}

	if(revents_ & EPOLLOUT) {
		if (writeCallback_)
			writeCallback_();
	}

	eventHandling_ = false;
}

void Channel::setReadCallback(const EventCallback& cb) {
	readCallback_ = cb;
}

void Channel::setWriteCallback(const EventCallback& cb) {
	writeCallback_ = cb;
}

void Channel::setErrorCallback(const EventCallback& cb) {
	errorCallback_ = cb;
}

void Channel::setCloseCallback(const EventCallback& cb) {
	closeCallback_ = cb;
}


int Channel::fd() const {
	return fd_;
}

int Channel::events() const {
	return events_;
}

int& Channel::revents() {
	return revents_;
}

const int& Channel::revents() const {
	return revents_;
}

void Channel::set_revents(int revt) {
	revents_ = revt;
}

bool Channel::isNoneEvent() const {
	return events_ == kNoneEvent;
}


void Channel::remove() {
	loop_->removeChannel(*this);
}



void Channel::enableReading() {
	events_ |= kReadEvent;
	update();
}

void Channel::enableWriting() {
	events_ |= kWriteEvent;
	update();
}

void Channel::enableReadAndWrite() {
	events_ |= kReadEvent;
	events_ |= kWriteEvent;
	update();
}

bool Channel::isReading() const {
	return revents_ & kReadEvent;
}

bool Channel::isWriting() const {
	return revents_ & kWriteEvent;
}


void Channel::disableReading() {
	events_ &= ~kReadEvent;
	update();
}

void Channel::disableWriting() {
	events_ &= ~kWriteEvent;
	update();
}

void Channel::disableReadAndWrite() {
	events_ &= ~kReadEvent;
	events_ &= ~kWriteEvent;
	update();
}



//for Poller
int Channel::index() const{
	return index_;
}

void Channel::set_index(int idx) {
	index_ = idx;
}

EventLoop* Channel::ownerLoop() {
	return loop_;
}


void Channel::update() {
	if (loop_->hasChannel(*this)) {
		loop_->updateChannel(*this);
	}
	else
		loop_->insertChannel(*this);
}

std::string Channel::toString() {
	return eventToString(fd_, events_);
}


std::string Channel::eventToString(int fd, int event) {
	std::string str = "fd = " + std::to_string(fd) + ", event = ";
	if (event & EPOLLIN)
		str += "EPOLLIN ";
	if (event & EPOLLOUT)
		str += "EPOLLOUT ";
	if (event & EPOLLERR)
		str += "EPOLLERR ";
	if (event & EPOLLPRI)
		str += "EPOLLPRI ";
	if (event & EPOLLHUP)
		str += "EPOLLHUP ";
	if (event & EPOLLRDHUP)
		str += "EPOLLRDHUP ";
	return str;
}