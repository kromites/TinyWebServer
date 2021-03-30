#include "./base/Logger.h"
#include "Address.h"
#include "Socket.h"
#include "Poller.h"
#include "Buffer.h"

#include <strings.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>


constexpr int MaxSize = 4096;
constexpr int MaxEvents = 20;


void addNewConn(Poller& poller, int listenfd) {
	LOG_TRACE << "add new conn";
	while(true) {
		std::pair<int, Address> ret = Accept(listenfd);
		int connfd = ret.first;	
		Address peer = ret.second;

		if (connfd > 0) {
			setNonBlock(connfd);
			LOG_INFO << "new connection from " << peer.toString();
		}
		else {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				LOG_DEBUG << "accept will be block";
				break;
			}
			else {
				LOG_ERROR << "accept returns error from connection fd : " << connfd;
				break;
			}
		}
		
		// on connection handle the epoll_event
		epoll_event event{};
		bzero(&event, sizeof event);

		event.events = EPOLLIN;
		event.data.fd = connfd;
		poller.add_fd(event);
	}
}

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
	
	poller.del_fd(event);
}

void handleRead(int connfd, Poller& poller) {
	LOG_TRACE << "handle read";
	// char Buffer[MaxSize];
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


int main() {
	auto servAddr = Address::getListenAddress(23456);

	Logger::setLogLevel(Logger::LogLevel::DEBUG);
	
	auto listenfd = tcpSocket();

	LOG_INFO << "listenfd : " << listenfd;
	
	Bind(listenfd, servAddr);

	Listen(listenfd, 10);
	// todo add class buffer instead of char* buffer
	// Buffer buffer;
	
	epoll_event ev;
	std::vector<epoll_event> events;
	Poller poller;

	
	bzero(&ev, sizeof ev);
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;
	
	poller.add_fd(ev);
	
	LOG_INFO << "server running";
	while(true) {
		LOG_DEBUG << "new round start";
		const auto numEvents = poller.poll(events, -1);
		for(int i=0; i < numEvents; ++i) {
			if(events[i].data.fd == listenfd) {
 				addNewConn(poller, listenfd);
			}else {
				handleRead(events[i].data.fd, poller);
			}
		}
	}
	sleep(60);
}

// todo 21/3/18: complete the logger, and make the epoll to manage the connection


// todo 21/3/19: build a socket.h and a address.h, encapsulate the functions about two class


// todo 21/3/20 complete socket.h address.h and make the Poll.h in the link list


// todo 21/3/24 start make the IO non-blocking

// todo read and right become IO non-block