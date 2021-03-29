#pragma once
#include "./base/Logger.h"

#include <vector>
#include <sys/epoll.h>
#include <unordered_map>


class Poller {
private:
	std::unordered_map<int, epoll_event> eventsMap_;
	std::vector<epoll_event> events_;        // epoll_wait
	int epollfd_;                            // epoll_create

	void poll_ctl(epoll_event, int) const;
	void fillActiveEvents(int, std::vector<epoll_event>&) const; 
public:
	Poller();
	~Poller();

	int poll(std::vector<epoll_event>&, int);
	
	int getEpollfd() const {
		return epollfd_;
	}

	bool checkFdInEpoll(int);
	
	void add_fd(epoll_event);
	void del_fd(epoll_event);
	void mod_fd(epoll_event);
};