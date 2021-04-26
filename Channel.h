#pragma once

#include <functional>

class EventLoop;

/*
 * Channel object manage the IO events of One fd.
 * but this object don't own this fd, and when it destors, the fd isn't closed.
 * function: Channel object can distribute different IO events as different callbacks. suck as readCallback, writeCallback.
 *
 * feature: channel member function can only run in IO thread, so update the member variable without lock.
 */

class Channel {
public:
	typedef std::function<void()> EventCallback;

	Channel(EventLoop* loop, int fd);

	void handleEvent();
	void setReadCallback(const EventCallback& cb);
	void setWriteCallback(const EventCallback& cb);
	void setErrorCallback(const EventCallback& cb);
	void setCloseCallback(const EventCallback& cb);

	int fd() const;
	int events() const;
	int& revents();
	const int& revents() const;
	void set_revents(int revt);
	bool isNoneEvent() const;

	void remove();
	
	void enableReading();
	void enableWriting();
	void enableReadAndWrite();

	bool isReading() const;
	bool isWriting() const;

	void disableReading();
	void disableWriting();
	void disableReadAndWrite();
	
	// for Poller

	// what the mean of index ?
	int index() const;
	void set_index(int idx);

	EventLoop* ownerLoop();

	// for log format
	std::string toString();

	std::string eventToString(int fd, int event);

private:
	void update();

	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	EventLoop* loop_;
	const int fd_;
	int events_;              /* IO events */
	int revents_;             /* careful IO event */
	int index_;               /* used by Poller */

	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback errorCallback_;
	EventCallback closeCallback_;
};
