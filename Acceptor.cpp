#include "Acceptor.h"

USE_NAMESPACE

Acceptor::Acceptor(EventLoop* loop, uint16_t port, const acceptCallback& callback)
	:peerAddress("0.0.0.0", 0),
	hostAddress("127.0.0.1", port),
	listenFd_(tcpSocket()),
	acceptCallback_(callback),
	loop_(loop),
	listenChannel_(loop, listenFd_),
	address_(Address::getListenAddress(port)),
	listenning_(false)
{
	LOG_TRACE << "Acceptor created, listenFd = " << listenFd_;
	Bind(listenFd_, address_);
	
	// set readable for channel;
	listenChannel_.enableReading();
	// bind the callback in read
	listenChannel_.setReadCallback([this]() { this->onAccept(); });
}


Acceptor::~Acceptor() {
	// set from readable to disable all for channel
	// only server disconnects, this desctor would be called
	listenChannel_.disableReadAndWrite();
	listenChannel_.remove();
	Close(listenFd_);
}

void Acceptor::listen(int backlog) {
	if(listenning_) {
		LOG_FATAL << "sockfd is already in listening";
	}
	listenning_ = true;
	Listen(listenFd_, backlog);
}


void Acceptor::onAccept() {
	LOG_TRACE << "add new conn";
	while (true) {
		const auto connfd = Accept(listenFd_, peerAddress);
		if (connfd > 0) {
			setNonBlock(connfd);
			LOG_INFO << "new connection from " << peerAddress.toString();
			acceptCallback_(connfd);
		}
		else {
			if (errno == EAGAIN || errno == EWOULDBLOCK) {
				LOG_DEBUG << "accept will be blocked";
				break;
			}
			else {
				LOG_ERROR << "accept returns error from connection fd : " << connfd;
				break;
			}
		}		
	}
}

int Acceptor::getListenFd() {
	return listenFd_;
}

void Acceptor::setFuncPtr(const acceptCallback& func) {
	acceptCallback_ = func;
}

