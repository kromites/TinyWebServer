#include "Poller.h"

#include "./base/Logger.h"

#include <cassert>
#include <cstdlib>
#include <strings.h>

Poller::Poller(): events_(10) {
	LOG_TRACE << "start create epoll ";
	epollfd_ = epoll_create1(0);
	if (epollfd_ < 0)
		LOG_ERROR << "create epoll errors";
}

Poller::~Poller() {
	
}

void Poller::poll_ctl(epoll_event ev , int opt) const {
	int ret;
	LOG_DEBUG << "epoll ctl: "<<opt<< " epollfd_ : " << epollfd_ << "  ev.data.fd :" << ev.data.fd;
 	if (opt == EPOLL_CTL_DEL) {
		ret = epoll_ctl(epollfd_, opt, ev.data.fd, nullptr);
	}
	else {
		// epoll_event event{};
		// epoll_event event{};
		// bzero(&event, sizeof event);
		// event.events = ev.events | EPOLLET;              // edge-triggered
		// event.data.fd = ev.data.fd;  

		ret = epoll_ctl(epollfd_, opt, ev.data.fd, &ev);
	}

	if (ret < 0)
		LOG_ERROR << "the return of poll_ctl is error";
}

void Poller::fillActiveEvents(int activeNumEvents, std::vector<epoll_event>& activeEvents) const {
	assert(activeNumEvents <= static_cast<int>(events_.size()));
	activeEvents.clear();
	for (int i = 0; i < activeNumEvents; ++i) {
		activeEvents.push_back(events_[i]);
	}
}

int Poller::poll(std::vector<epoll_event>& activeEvents, int timeouts) {
	const auto activeNumEvents = epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), timeouts);

	LOG_INFO << "epoll_wait return " << activeNumEvents << " events active";
	if (activeNumEvents == -1) {
		LOG_ERROR << "epoll_wait error";
	}
	else if (activeNumEvents == 0) {
		LOG_INFO << "no connection accept";
	}
	else {
		fillActiveEvents(activeNumEvents, activeEvents);
		if(events_.size() == activeNumEvents) {
			LOG_INFO << "epoll vector reserve double";
			events_.reserve(activeNumEvents * 2);
		}
	}
	return activeNumEvents;
}

bool Poller::checkFdInEpoll(int fd) {
	return eventsMap_.count(fd);
}

void Poller::add_fd(epoll_event ev) {
	LOG_TRACE << "insert a event in the epoll vector";
	poll_ctl(ev, EPOLL_CTL_ADD);
	auto fd = ev.data.fd;
	eventsMap_[fd] = ev;
}

void Poller::del_fd(epoll_event ev) {
	LOG_TRACE << "delete a event in the epoll vector";
	poll_ctl(ev, EPOLL_CTL_DEL);
	eventsMap_.erase(ev.data.fd);
}

void Poller::mod_fd(epoll_event ev) {
	LOG_TRACE << "modify a event in the epoll vector";
	poll_ctl(ev, EPOLL_CTL_MOD);
}



