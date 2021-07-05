// int vec;
// void push_back(int fd, int val);
// void pop_back(int fd);
// void clear(int fd);
// int init_vector(int size);

// how to use it?
// auto vec = init_vector(10);
// push_back(vec, 1);
// push_back(vec, 2);
// push_back(vec, 3);
// clear(vec);

/*
class vector {
public:
	vector(int size) : realVec_(init_vector(size)){}

	void push_back(int val){
		::push_back(realVec_, val);
	}


private:
	int realVec_;
};

*/


// using vector base on object
// vector vec(2);
// vec.push_back(10);
#pragma once

#include "base/Util.h"

#include "Socket.h"
#include "Channel.h"
#include <functional>
//
// class Promise {
// public:
// 	Promise(std::function<void(int)> set): set_(set) {
// 	}
//
// 	void run(int arg) {
// 		// operation after set.
// 		set_(arg);
// 	}
//
// private:
// 	std::function<void(int)> set_;
// };

// feature: get new connection
// no-feature : add new connection to epoll
START_NAMESPACE

class EventLoop;
class Channel;

class Acceptor {
public:
	typedef std::function<void(int)> acceptCallback;
	// Acceptor: get listenFd
	Acceptor(EventLoop* loop, uint16_t port, const acceptCallback& callback = nullptr);

	~Acceptor();

	// call sockets api
	void listen(int length);
	// get an integer and an address from sys call `::Accept()`
	void onAccept();

	// make a connfd

	// get listenFd
	int getListenFd();
	
	void setFuncPtr(const acceptCallback& func);

	mutable Address peerAddress;
	Address hostAddress;

private:
	int listenFd_;
	acceptCallback acceptCallback_;

	// adapt the eventloop and channel
	EventLoop* loop_;
	Channel listenChannel_;

	// listenAddress
	Address address_;
	
	mutable bool listenning_;
};

END_NAMESPACE