#include "Socket.h"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/eventfd.h>

USE_NAMESPACE

int tinyWebSever::tcpSocket() {
	const auto res = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, 0);
	if (res < 0)
		LOG_ERROR << "socket error, can't generate listenfd";
	return res;
}

void tinyWebSever::Bind(int listenfd, Address addr) {
	LOG_TRACE << "start bind with " << listenfd;
	auto ret = bind(listenfd, addr.getAddr(), addr.size());
	if (ret < 0)
		LOG_ERROR << "bind error";
}

void tinyWebSever::Listen(int listenfd, int maxLength) {
	LOG_TRACE << "start listen from " << listenfd;
	auto ret = listen(listenfd, maxLength);
	if (ret < 0)
		LOG_ERROR << "listen error";
}


void tinyWebSever::setNonBlock(int fd) {
	auto flag = fcntl(fd, F_GETFL, 0);
	if (flag < 0)
		LOG_ERROR <<"fd = " << fd << " fcntl F_GETFL errors";

	flag |= O_NONBLOCK;

	if(fcntl(fd, F_SETFL, flag) < 0)
		LOG_ERROR << "fcntl F_SETFL O_NONBLOCK errors";

	if(flag = fcntl(fd, F_GETFD, 0) < 0)
		LOG_ERROR << "fcntl F_GETFD errors";
	flag |= O_CLOEXEC;

	if (fcntl(fd, F_SETFD, flag) < 0)
		LOG_ERROR << "fcntl F_SETFD O_CLOEXEC errors";

}

int tinyWebSever::Accept(int sockfd, Address& addr) {
	socklen_t addrlen = sizeof addr;
	const int connfd = accept(sockfd, addr.getAddr(), &addrlen);
	const int curErrno = errno;
	if (connfd > 0) {
		setNonBlock(connfd);
		return connfd;
	}
	if (curErrno == EINTR) {
		return Accept(sockfd, addr);
	}
	if ((curErrno == EWOULDBLOCK) || (curErrno == EAGAIN)) {
		LOG_TRACE << "accept will be blocked";
		return 0;
	}
	LOG_FATAL << "accept fault";
	return -1;
}

int tinyWebSever::eventFd() {
	int eventfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (eventfd < 0) {
		LOG_FATAL << "Failed in generating event fd";
	}
	return eventfd;
}

void tinyWebSever::Close(int sockfd) {
	close(sockfd);
}
