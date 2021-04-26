#include "Poller.h"
#include "Channel.h"
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

epoll_event Poller::createEvent(int fd) const {
	epoll_event event{};
	bzero(&event, sizeof event);
	event.events = EPOLLIN;
	event.data.fd = fd;
	return event;
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

void Poller::add_fd(int fd) {
	LOG_TRACE << "insert a event in the epoll vector";
	epoll_event ev = createEvent(fd);
	poll_ctl(ev, EPOLL_CTL_ADD);
	eventsMap_[fd] = ev;
}

void Poller::del_fd(int fd) {
	LOG_TRACE << "delete a event in the epoll vector";
	epoll_event ev = createEvent(fd);
	poll_ctl(ev, EPOLL_CTL_DEL);
	eventsMap_.erase(fd);
}

void Poller::mod_fd(int fd) {
	LOG_TRACE << "modify a event in the epoll vector";
	epoll_event ev = createEvent(fd);
	poll_ctl(ev, EPOLL_CTL_MOD);
}

/*
 *	Use Channel to replace the epoll_event;
 *	core function -> poll(ChannelList, timeouts)
 *	function : deal with channel : update, insert, remove -> poll_ctl(channel, option)
 *			   judge channel whether in map : hasChannel -> bool
 */

//todo replace INT with TIMESTAMP
int Poller::poll(std::vector<Channel*>& activeChannels, int timeouts) {
	const auto activeNumEvents = epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), timeouts);

	LOG_INFO << "epoll_wait return " << activeNumEvents << " events active";
	if (activeNumEvents == -1) {
		LOG_ERROR << "epoll_wait error";
	}
	else if (activeNumEvents == 0) {
		LOG_INFO << "no connection accept";
	}
	else {
		fillActiveChannels(activeNumEvents, activeChannels);
		if (events_.size() == activeNumEvents) {
			LOG_INFO << "epoll vector reserve double";
			events_.reserve(activeNumEvents * 2);
		}
	}
	return activeNumEvents;
}



void Poller::insert(Channel& channel) {
	LOG_TRACE << "insert channel: " << channel.toString();
	const int fd = channel.fd();
	poll_ctl(channel, EPOLL_CTL_ADD);
	channelMap_[fd] = &channel;
}

void Poller::remove(Channel& channel) {
	LOG_TRACE << "remove channel: " << channel.toString();
	const int fd = channel.fd();
	poll_ctl(channel, EPOLL_CTL_DEL);
	channelMap_.erase(fd);
}

void Poller::update(Channel& channel) {
	LOG_TRACE << "update channel: " << channel.toString();
	poll_ctl(channel, EPOLL_CTL_MOD);
}

bool Poller::hasChannel(Channel& channel) {
	auto it = channelMap_.find(channel.fd());
	return it != channelMap_.end();
}


void Poller::poll_ctl(Channel& channel, int opt) const {
	int ret;
	LOG_DEBUG << "epoll ctl: " << opt << " epollfd_ : " << epollfd_ << "  ev.data.fd :" << channel.fd();
	if (opt == EPOLL_CTL_DEL) {
		ret = epoll_ctl(epollfd_, opt, channel.fd(), nullptr);
	}
	else {
		epoll_event event{};
		bzero(&event, sizeof event);
		event.events = channel.events() | EPOLLET;              // edge-triggered
		event.data.ptr = const_cast<Channel*>(&channel);

		ret = epoll_ctl(epollfd_, opt, channel.fd(), &event);
	}

	if (ret < 0)
		LOG_FATAL << "the return of poll_ctl is error";
}


void Poller::fillActiveChannels(int activeCount, std::vector<Channel*>& activeChannels) const {
	assert(activeCount <= static_cast<int>(events_.size()));
	LOG_TRACE << "epoll_wait return, " << activeCount << " events active";
	for (int i = 0; i < activeCount; ++i) {
		const auto& event = events_[i];
		Channel* channel = static_cast<Channel*>(event.data.ptr);
		LOG_DEBUG << channel->toString();
		
		channel->set_revents(event.events) ;
		activeChannels.push_back(channel);
	}
}
