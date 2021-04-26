#include "Acceptor.h"


Acceptor::Acceptor(int port, _func func) : listenFd_(tcpSocket()), set_(func) {
	LOG_TRACE << "Acceptor created, listenFd = " << listenFd_;
	Bind(listenFd_, Address::getListenAddress(port));
	Listen(listenFd_, 10);
}

Acceptor::~Acceptor() {
}

void Acceptor::onAccept() {
	LOG_TRACE << "add new conn";
	while (true) {
		std::pair<int, Address> ret = Accept(listenFd_);
		int connfd = ret.first;
		Address peer = ret.second;

		// set_(connfd);		/* for test the function */
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
		// run add connfd into poll
		set_(connfd);
	}
}

int Acceptor::getListenFd() {
	return listenFd_;
}

void Acceptor::setFuncPtr(_func& func) {
	set_ = func;
}
