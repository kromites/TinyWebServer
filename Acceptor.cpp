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
	listenChannel_.setReadCallback(std::bind(&Acceptor::onAccept, this));
}


Acceptor::~Acceptor() {
	// set from readable to disable all for channel
	listenChannel_.disableReadAndWrite();
	listenChannel_.remove();

	// close listenfd;
	Close(listenFd_);
}

void Acceptor::listen(int length) {
	if(listenning_) {
		LOG_FATAL << "sockfd is already in listening";
	}
	listenning_ = true;
	Listen(listenFd_, length);
}


void Acceptor::onAccept() {
	LOG_TRACE << "add new conn";
	while (true) {
		Address peer;
		
		int connfd = Accept(listenFd_, peer);
		
		// set_(connfd);		/* for test the function */
		if (connfd > 0) {
			setNonBlock(connfd);
			LOG_INFO << "new connection from " << peer.toString();
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
		// run add connfd into poll
		acceptCallback_(connfd);
	}
}

int Acceptor::getListenFd() {
	return listenFd_;
}

void Acceptor::setFuncPtr(const acceptCallback& func) {
	acceptCallback_ = func;
}

