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
#include "Socket.h"
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
class Acceptor {
public:
	typedef std::function<void(int)> _func;
	// Acceptor: get listenFd
	Acceptor(int, _func);

	// do nothing
	~Acceptor();

	// get an integer and an address from sys call `::Accept()`
	void onAccept();

	// make a connfd

	// get listenFd
	int getListenFd();
	
	void setFuncPtr(_func& func);

private:
	int listenFd_;
	_func set_;
};


