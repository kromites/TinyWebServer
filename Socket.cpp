#include "Socket.h"
#include <arpa/inet.h>
#include <fcntl.h>


int tcpSocket() {
	const auto res = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	if (res < 0)
		LOG_ERROR << "socket error, can't generate listenfd";
	return res;
}

void Bind(int listenfd, Address addr) {
	LOG_TRACE << "start bind with " << listenfd;
	auto ret = bind(listenfd, addr.getAddr(), addr.size());
	if (ret < 0)
		LOG_ERROR << "bind error";
}

void Listen(int listenfd, int maxLength) {
	LOG_TRACE << "start listen from " << listenfd;
	auto ret = listen(listenfd, maxLength);
	if (ret < 0)
		LOG_ERROR << "listen error";
}


void setNonBlock(int fd) {
	auto flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0)
		LOG_ERROR << "fcntl F_GETFL errors";

	flag |= O_NONBLOCK;
	// close fd when exec() called
	if(fcntl(fd, F_SETFL, flag) < 0)
		LOG_ERROR << "fcntl F_SETFL O_NONBLOCK errors";

	if(flag = fcntl(fd, F_GETFD, 0) < 0)
		LOG_ERROR << "fcntl F_GETFD errors";
	flag |= O_CLOEXEC;

	if (fcntl(fd, F_SETFD, flag) < 0)
		LOG_ERROR << "fcntl F_SETFD O_CLOEXEC errors";

}


std::pair<int, Address> Accept(int fd) {
	Address clientAddr;
	socklen_t len = sizeof clientAddr;
	LOG_TRACE << "start accept from " << fd;
	const auto connfd = accept(fd, clientAddr.getAddr(), &len);
	
	return { connfd, clientAddr };
}
