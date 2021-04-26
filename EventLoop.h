#pragma once

#include <vector>
#include <memory>

#include "Channel.h"
class Poller;
class Channel;

class EventLoop {
public:
	EventLoop();
	~EventLoop();

	void loop();

	void assertInLoopThread() const;

	bool isInLoopThread() const;

	EventLoop* getEventLoopOfCurrentThread();

	void quit();

	// for poller to manage channel

	void insertChannel(Channel& channel) const;
	void updateChannel(Channel& channel) const;
	void removeChannel(Channel& channel) const;

	bool hasChannel(Channel& channel) const;
private:
	void abortNotInLoopThread() const;

	typedef std::vector<Channel*> ChannelList;

	static int KEpollTimeMs;
	
	bool looping_;  /* atomic */
	bool quit_;  /* atomic */
	const pid_t threadId_;
	std::unique_ptr<Poller> poller_;
	ChannelList activeChannels_;
};
