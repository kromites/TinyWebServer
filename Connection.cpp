#include "Connection.h"

#include <cassert>
#include <unistd.h>

USE_NAMESPACE

Connection::Connection(EventLoop& loop, int fd, const Address& hostAddr, Address& peerAddr):
	loop_(&loop), hostAddr_(hostAddr), peerAddr_(peerAddr), channel_(loop_, fd)
{
	channel_.setReadCallback([this]() { this->handleRead(); });
	channel_.setWriteCallback([this]() { this->handleWrite(); });
}


Connection::~Connection() {
	
}

void Connection::handleRead() {
	LOG_TRACE << "start handle read";
	char buf[65536];
	
	while (true) {
		const auto readLen = ::read(channel_.fd(), buf, sizeof buf);
		// FIXME: close connection if n == 0;
		if (readLen > 0) {
			inputBuffer_.write(buf, readLen);
			LOG_TRACE << "read message from connection: " << channel_.fd() << " and length is " << readLen;
			LOG_DEBUG << "inputBuffer string: " << inputBuffer_.readString();
			messagecallback_(shared_from_this(), inputBuffer_);
		}
		else if (readLen == 0) {
			close();
			break;
		}
		else {
			if(errno == EINTR) {
				continue;
			}else if(errno == EWOULDBLOCK || EAGAIN) {
				LOG_TRACE << "keep read will be block, waiting for next read event";
				break;
			}
			else {
				handleError();
				LOG_ERROR << "Connection::handleRead error in fd " << channel_.fd();
				break;
			}
		}
	}
}

// Epoll edge trigger, so we need write until empty or EAGAIN. 
void Connection::handleWrite() {
	loop_->assertInLoopThread();
	LOG_TRACE << "start handle write";
	while (true) {
		if (channel_.isWriting()) {
			const auto writeLen = write(channel_.fd(), outputBuffer_.readString(), outputBuffer_.ReadableSize());
			if (writeLen > 0) {
				LOG_TRACE << "write message from connection: " << channel_.fd() << " and length is " << writeLen;
				outputBuffer_.consume(writeLen);
			}
			if(outputBuffer_.empty()) {
				channel_.disableWriting();
				LOG_TRACE << "all write buffer sent";
				if(writeCompleteCallback_) {
					loop_->queueInLoop([this]() {
						this->writeCompleteCallback_(shared_from_this());
						});
				}
				break;
			}
			if(writeLen == -1) {
				if(errno == EAGAIN || errno == EWOULDBLOCK) {
					LOG_TRACE << "keep write will be block, waiting for next write event";
				}else if(errno == EINTR) {
					continue;
				}else {
					handleError();
					LOG_ERROR << "Connection::handleWrite error in fd " << channel_.fd();
				}
				
			}
		}
		else {
			LOG_TRACE << "Connection is down, can't writing";
			break;
		}
	}
}


// main feature : call the closeCallback_
// void Connection::handleClose() {
// 	loop_->assertInLoopThread();
// 	LOG_TRACE << "TcpConnection::handleClose state = " << state_;
// 	assert(state_ == kConnected || kDisconnecting );
// 	state_ = kDisconnected; 
// 	// we don't close fd, leave it to dctor, so we can find leaks easily.
// 	channel_.disableReadAndWrite();
// 	channel_.remove();
// 	::shutdown(channel_.fd(), SHUT_RDWR);
// 	// must be the last line
// 	closecallback_(shared_from_this());
// }


void Connection::handleError() {
	close();
}

void Connection::connectEstablished() {
	loop_->assertInLoopThread();
	assert(state_ == kConnecting);
	setState(kConnected);

	// set fd;
	channel_.enableReading();
	connectioncallback_(shared_from_this());
}

void Connection::connectDestroyed() {
	loop_->assertInLoopThread();
	assert(state_ == kConnected || kDisconnecting);
	setState(kDisconnected);
	channel_.disableReadAndWrite();
	connectioncallback_(shared_from_this());

	loop_->removeChannel(channel_);
}

void Connection::close() {
	// close -> channel close
	//		 -> state -> disconnected;

	channel_.disableReadAndWrite();
	channel_.remove();
	::shutdown(channel_.fd(), SHUT_RDWR);
	state_ = kDisconnected;
	LOG_DEBUG << "test close : state_ = " << state_;
	loop_->queueInLoop([this]() {
		closecallback_(shared_from_this());
		});
}


void Connection::send(std::string& message) {
	if(state_ == kConnected) {
		if(loop_->isInLoopThread()) {
			// send message in IO thread
			sendInloop(message);
		}else {
			loop_->runInLoop([this, message]() { this->sendInloop(message); });
		}
	}
}

//thread safe, call shutdownInLoop()
void Connection::shutdown() {
	// FIXME: use compare and swap
	if(state_ == kConnected) {
		setState(kConnecting);
		loop_->runInLoop([this]() { this->close(); });
	}
}


void Connection::sendInloop(const std::string& message) {
	loop_->assertInLoopThread();
	ssize_t nwrote = 0;
	// if nothing in output queue, try writing directly
	if(!channel_.isWriting() && outputBuffer_.ReadableSize() == 0) {
		nwrote = ::write(channel_.fd(), message.data(), message.size());
		if(nwrote >= 0) {
			if(static_cast<size_t>(nwrote) < message.size()) {
				LOG_TRACE << "I am going to write more data";
			}
		}else {
			nwrote = 0;
			if(errno != EWOULDBLOCK) {
				LOG_ERROR << "Connection::sendInLoop";
			}
		}
	}

	assert(nwrote >= 0);
	if(static_cast<size_t>(nwrote) < message.size()) {
		outputBuffer_.write(message.data() + nwrote, message.size() - nwrote);
		if(!channel_.isWriting()) {
			channel_.enableWriting();
		}
	}
	
}

const bool Connection::connected() const{
	return state_ == kConnected;
}

const Address Connection::hostAddr() const {
	return hostAddr_;
}

const Address Connection::peerAddr() const {
	return peerAddr_;
}

// void Connection::shutdownInloop() {
// 	loop_->assertInLoopThread();
// 	if(!channel_.isWriting()) {
// 		//todo shutdown writing.
// 	}
// }
