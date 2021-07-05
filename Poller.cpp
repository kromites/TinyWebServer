#include "Poller.h"
#include "Channel.h"
#include "./base/Logger.h"

#include <cassert>
#include <cstdlib>
#include <strings.h>

USE_NAMESPACE

Poller::Poller(): events_(10) {
	LOG_TRACE << "start create epoll ";
	epollfd_ = epoll_create1(0);
	if (epollfd_ < 0)
		LOG_ERROR << "create epoll errors";
}

Poller::~Poller() {
	
}

bool Poller::checkFdInEpoll(int fd) {
	return eventsMap_.count(fd);
}

/*
 *	Use Channel to replace the epoll_event;
 *	core function -> poll(ChannelList, timeouts)
 *	function : deal with channel : update, insert, remove -> poll_ctl(channel, option)
 *			   judge channel whether in map : hasChannel -> bool
 */

//todo replace INT with TIMESTAMP
int Poller::poll(std::vector<Channel*>& activeChannels, int timeouts) {
	LOG_TRACE << "the number of events in poller : " << channelMap_.size();
	
	const auto activeNumEvents = epoll_wait(epollfd_, events_.data(), static_cast<int>(events_.size()), timeouts);
	const auto err = errno;
	if (activeNumEvents == -1) {
		if (err != EINTR) {
			errno = err;
			LOG_FATAL << "Poller::poll()";
		}
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
	LOG_DEBUG << "epoll ctl: " << opt << " epollfd_ : " << epollfd_ << ", ev.data.fd : " << channel.fd();
	if (opt == EPOLL_CTL_DEL) {
		ret = epoll_ctl(epollfd_, opt, channel.fd(), nullptr);
		LOG_DEBUG << "test delete : ret = " << ret;
	}
	else {
		epoll_event event{};
		bzero(&event, sizeof event);
		event.events = channel.events() | EPOLLET;              // edge-triggered
		event.data.ptr = const_cast<Channel*>(&channel);

		ret = epoll_ctl(epollfd_, opt, channel.fd(), &event);
	}

	if (ret < 0) {
		LOG_FATAL << "epoll_ctl flag = " << flagToString(opt) << " fd = " << channel.fd();
	}
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

std::string Poller::flagToString(int flag) const {
	switch (flag) {
	case EPOLL_CTL_ADD:
		return "EPOLL_CTL_ADD";
	case EPOLL_CTL_DEL:
		return "EPOLL_CTL_DEL";
	case EPOLL_CTL_MOD:
		return "EPOLL_CTL_MOD";
	default:
		return "Unknown Operation";
	}
}