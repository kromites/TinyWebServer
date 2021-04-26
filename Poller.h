#pragma once
#include "./base/Logger.h"

#include <vector>
#include <sys/epoll.h>
#include <unordered_map>

class Channel;

class Poller {
public:
	Poller();
	~Poller();

	int poll(std::vector<epoll_event>&, int);
	
	int getEpollfd() const {
		return epollfd_;
	}

	bool checkFdInEpoll(int);
	
	void add_fd(int fd);
	void del_fd(int fd);
	void mod_fd(int fd);

	// channel
	int poll(std::vector<Channel*>&, int);
	
	void insert(Channel& channel);
	void remove(Channel& channel);
	void update(Channel& channel);
	bool hasChannel(Channel& channel);

private:
	void poll_ctl(epoll_event, int) const;
	void fillActiveEvents(int, std::vector<epoll_event>&) const;
	epoll_event createEvent(int fd) const;

	std::unordered_map<int, epoll_event> eventsMap_;
	std::vector<epoll_event> events_;        // epoll_wait
	int epollfd_;                            // epoll_create


	// channel
	std::unordered_map<int, Channel*> channelMap_;
	void poll_ctl(Channel&, int) const;
	void fillActiveChannels(int, std::vector<Channel*>&) const;
};