#include "TcpServer.h"
#include "Connection.h"

USE_NAMESPACE

TcpServer::TcpServer(EventLoop* loop, int port, int workthread) :
	mainLoop_(loop),
	acceptor_(mainLoop_, port, [this](int connfd) { this->newConnection(connfd); }),
	threadPool_([](EventLoop* loop) { LOG_INFO << "thread init"; }, workthread, "ioThread"),
	name_(threadPool_.threadName())
{
}

TcpServer::~TcpServer() {
	LOG_TRACE << "TcpServer destructor";
}

// create the connection
void TcpServer::start() {
	LOG_INFO << "Tcp Server Start";
	threadPool_.start();

	acceptor_.listen(256);
	mainLoop_->loop();
}


void TcpServer::newConnection(int sockfd) {
	mainLoop_->assertInLoopThread();
	EventLoop* ioloop = threadPool_.getNextLoop();
	char buf[32];
	snprintf(buf, sizeof buf, "#%d", nexConnId_);
	++nexConnId_;
	std::string connName = this->name_;
	connName += static_cast<std::string>(buf);

	// fixme: question -> acceptor_.peerAddress.toString(), peerAddress is "0.0.0.0", why can to string?
	LOG_INFO << "TcpServer::newConnection [" << name_ << "] - new connnection [" << connName << "] from "
		<< acceptor_.peerAddress.toString();

	// fixme: param
	TcpConnectionPtr conn = std::make_shared<Connection>(*ioloop, sockfd, acceptor_.hostAddress, acceptor_.peerAddress );
	connections_[connName] = conn;

	// conn
	conn->setConnectionCallback(connectioncallback_);
	conn->setMessageCallback([this](TcpConnectionPtr conn, Buffer& buffer) { this->messagecallback_(conn, buffer); });
	conn->setCloseCallback([this](std::shared_ptr<Connection> conn) {
		removeConnection(conn);
		});

	conn->setWriteCompleteCallback([this](std::shared_ptr<Connection> conn) { onWriteFinish(conn); });

	// fixme
	// conn->connectEstablished();
	ioloop->runInLoop([conn]() { conn->connectEstablished(); });
}

// remove conn from ConnectionMap
void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
	LOG_INFO << "TcpServer::removeConnection [" << name_ << "]- connection " << conn->name();
	mainLoop_->queueInLoop([this] {
		this->connections_.erase(name_);
		});
	if(closecallback_) {
		closecallback_(conn);
	}
}


void TcpServer::onWriteFinish(const TcpConnectionPtr& conn) {
	mainLoop_->assertInLoopThread();
	LOG_INFO << "TcpServer::onWriteFinish [" << name_ << "]- connection " << conn->name();
	if(writefinishcallback_) {
		writefinishcallback_(conn);
	}
}
