#pragma once
#include <map>

#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "Address.h"
#include "Acceptor.h"
#include "Buffer.h"
/*
 * TcpServer class
 *			-> function : manage the TcpConnection from Acceptor
 *			-> feature : the Lifetime is controlled by user.
 *			-> usage : user only sets the callback function, and invoke TcpServer::start()
 */

class Connection;

class TcpServer {
private:
	using TcpConnectionPtr = std::shared_ptr<Connection>;
	using ConnectionCallback = std::function<void(TcpConnectionPtr)>;
	using MessageCallback = std::function<void(TcpConnectionPtr, Buffer&)>;
	using CloseCallback = std::function<void(TcpConnectionPtr)>;
	using WriteFinishCallback = std::function<void(TcpConnectionPtr)>;
	using ConnectionMap = std::map<std::string, TcpConnectionPtr>;
	
public:

	// FIXME add string name;
	TcpServer(EventLoop* loop, int port, int workthread);
	~TcpServer();                // force out-line dtoc, for unique_ptr members

	// Starts the server if it's not listenning

	// It's harmless to call it multiple times
	// Thread safe

	void start();

	// Set connection callback
	// Not thread safe
	void setConnectionCallback(const ConnectionCallback& cb) { connectioncallback_ = cb; }
	void setMessageCallback(const MessageCallback& cb) { messagecallback_ = cb; }

	

private:
	// Not thread safe, but in loop
	void newConnection(int sockfd);
	void removeConnection(const TcpConnectionPtr& conn);
	void onWriteFinish(const TcpConnectionPtr& conn);


	EventLoop* mainLoop_;
	
	Acceptor acceptor_;       // avoid revealing Acceptor
	EventLoopThreadPool threadPool_;
	
	ConnectionCallback connectioncallback_;
	MessageCallback messagecallback_;
	CloseCallback closecallback_;
	WriteFinishCallback writefinishcallback_;
	
	const std::string name_;
	bool started_;
	int nexConnId_;        // always in loop thread;
	ConnectionMap connections_;
	
};