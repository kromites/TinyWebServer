#include "./base/Logger.h"
#include "./base/Thread.h"

#include "Address.h"
#include "Acceptor.h"
#include "Socket.h"
#include "Poller.h"
#include "Buffer.h"

// #include <thread>
#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>


// void addNewConn(Poller& poller, int listenfd) {
// 	LOG_TRACE << "add new conn";
// 	while(true) {
// 		std::pair<int, Address> ret = Accept(listenfd);
// 		int connfd = ret.first;	
// 		Address peer = ret.second;
//
// 		if (connfd > 0) {
// 			setNonBlock(connfd);
// 			LOG_INFO << "new connection from " << peer.toString();
// 		}
// 		else {
// 			if (errno == EAGAIN || errno == EWOULDBLOCK) {
// 				LOG_DEBUG << "accept will be block";
// 				break;
// 			}
// 			else {
// 				LOG_ERROR << "accept returns error from connection fd : " << connfd;
// 				break;
// 			}
// 		}
// 		
// 		// on connection handle the epoll_event
// 		
// 		poller.add_fd(connfd);
// 	}
// }

void handleWrite(int connfd, Buffer buffer, int readlen) {
	while(true) {
		if (readlen <= 0) {
			LOG_TRACE << "finish write to connection fd : " << connfd;
			break;
		}
		const auto writeLen = write(connfd, buffer.readString(), readlen);
		if(writeLen > 0) {
			LOG_DEBUG << "write " << writeLen << "bytes to " << connfd;
			readlen -= writeLen;
			buffer.readSize(writeLen);
		}else {
			if(errno == EAGAIN || errno == EWOULDBLOCK) {
				LOG_DEBUG << "write will be block to connection fd : " << connfd;
				break;
			}else {
				LOG_ERROR << "write error to connection fd : " << connfd;
				break;
			}
		}
	}
}

void echoToPeer(int connfd, Buffer buffer, int readLen) {
	handleWrite(connfd, buffer, readLen);
}

void removeConnection(int connfd, Poller& poller) {
	shutdown(connfd, SHUT_RDWR);
	epoll_event event{};
	event.data.fd = connfd;
	event.events = EPOLLIN;
	
	poller.del_fd(connfd);
}

void handleRead(int connfd, Poller& poller) {
	LOG_TRACE << "handle read";
	Buffer buffer;
	
	while (true) {
		const auto readLen = read(connfd, buffer.writeStart(), buffer.size());
		if (readLen < 0) {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				LOG_DEBUG << "read will be block from connection fd : " << connfd;
				break;
			}else {
				LOG_ERROR << "read error from connection fd: " << connfd;
				removeConnection(connfd, poller);
				break;
			}
		}
		else if (readLen == 0) {
			LOG_INFO << "connection closed by peer";
			if (poller.checkFdInEpoll(connfd)) {
				removeConnection(connfd, poller);
			}else {
				break;
			}
		}
		else {
			// handle read:  first version we take a echo handle.
			buffer.writeSize(readLen);
			LOG_DEBUG << "read " << readLen << " bytes from " << connfd;
			echoToPeer(connfd, buffer, readLen);
		}
	}
	
}

std::function<void(int)> acceptCallback;

// no arg
void acceptThreadFunc(Acceptor& acceptor) {
	Poller acceptPoller;
	std::vector<epoll_event> events;
	acceptPoller.add_fd(acceptor.getListenFd());
	while (true) {
		 
		// get new connection
		// add new connection to epoll			
		acceptPoller.poll(events, -1);
		acceptor.onAccept();
	}
}


void ioThreadFunc(Poller& ioPoller) {
	while (true) {
		std::vector<epoll_event> events;
		auto numEvents = ioPoller.poll(events, -1);

		for (int i = 0; i < numEvents; ++i) {
			handleRead(events[i].data.fd, ioPoller);
		}
	}
}



int main() {
	
	Logger::setLogLevel(Logger::LogLevel::TRACE);
	Poller ioPoller;

	Thread t1([&ioPoller]() {
		ioThreadFunc(ioPoller);
	});
	t1.start();

	
	acceptCallback = [&ioPoller](int connfd) {
		ioPoller.add_fd(connfd);
	};
	Acceptor acceptor(23456, acceptCallback);
	// acceptor.setFuncPtr(acceptCallback);
	
	acceptThreadFunc(acceptor);
	

	t1.join();
	// auto servAddr = Address::getListenAddress(23456);
	//
	//
	// auto listenfd = tcpSocket();
	//
	// LOG_INFO << "listenfd : " << listenfd;
	//
	// Bind(listenfd, servAddr);
	//
	// Listen(listenfd, 10);
	//
	// std::vector<epoll_event> events;
	// Poller poller;
	//
	//
	// poller.add_fd(listenfd);
	//
	// LOG_INFO << "server running";
	// while(true) {
	// 	LOG_DEBUG << "new round start";
	// 	const auto numEvents = poller.poll(events, -1);
	// 	for(int i=0; i < numEvents; ++i) {
	// 		if(events[i].data.fd == listenfd) {
 // 				addNewConn(poller, listenfd);
	// 		}else {
	// 			handleRead(events[i].data.fd, poller);
	// 		}
	// 	}
	// }
	// sleep(60);
}

// todo 21/3/18: complete the logger, and make the epoll to manage the connection


// todo 21/3/19: build a socket.h and a address.h, encapsulate the functions about two class


// todo 21/3/20 complete socket.h address.h and make the Poll.h in the link list


// todo 21/3/24 start make the IO non-blocking

// todo read and write become IO non-block

// todo add feature in log, think how to make thread pool?

// todo fix the log_fatal, build eventloop, channel, and eventthread?