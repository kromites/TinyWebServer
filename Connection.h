#pragma once
#include <any>
#include <functional>
#include <memory>

#include "base/Util.h"
#include "TcpServer.h"
#include "Buffer.h"
#include "Channel.h"

START_NAMESPACE

class Channel;
class Connection : public std::enable_shared_from_this<Connection> {
private:
	using ConnectionPtr = std::shared_ptr<Connection>;
	using ConnectionCallback = std::function<void(ConnectionPtr)>;
	using MessageCallback = std::function<void(ConnectionPtr, Buffer&)>;
	using WriteCompleteCallback = std::function<void(ConnectionPtr)>;
	using CloseCallback = std::function<void(ConnectionPtr)>;

public:
	Connection(EventLoop& loop, int fd, const Address& hostAddr, Address& peerAddr);
	~Connection();


	const std::string name() const { return name_; }
	int fd() { return channel_.fd(); }

	// template <typename T>
	// void setContext(T& value) {
	// 	context_ = std::make_any<T>(value);
	// }
	//
	// std::any& getContext() {
	// 	return context_;
	// }

	//bug: copy from muduo, but still error 
	
	void setContext(const std::any& context)
	{
		context_ = context;
	}

	const std::any& getContext() const
	{
		return context_;
	}

	std::any* getMutableContext()
	{
		return &context_;
	}
	
	void setConnectionCallback(const ConnectionCallback& cb) { connectioncallback_ = cb; }
	void setMessageCallback(const MessageCallback& cb) { messagecallback_ = cb; }
	void setWriteCompleteCallback(const WriteCompleteCallback& cb) { writeCompleteCallback_ = cb; }
	void setCloseCallback(const CloseCallback& cb) { closecallback_ = cb; }

	// called when TcpServer accepts a new connection
	void connectEstablished();   // should be called only once

	// called when TcpServer has removed me from its map
	void connectDestroyed();

	void close();
	// add
	void send(const char* message, size_t len) {
		outputBuffer_.write(message, len);
		channel_.enableWriting();
	}
	//thread safe
	void send(std::string& message);
	void send(std::string&& message) { return send(message); }

	//thread safe
	void shutdown();
	void shutdownAndclose();

	void setTcpNoDelay(bool on);

	const bool connected() const;
	const Address hostAddr() const;
	const Address peerAddr() const;

	
private:
	enum StateE { kConnecting, kConnected, kDisconnecting, kDisconnected };
	void setState(StateE s) { state_ = s; }

	void handleRead();
	void handleWrite();
	// void handleClose();
	void handleError();

	//in ET: sendInloop is useful?
	void sendInloop(const std::string& message);
	void shutdownInloop();

	EventLoop* loop_;
	std::string name_;
	StateE state_; // FIXME use atomic variable

	// we don't expose those classes to client
	Channel channel_;
	Address hostAddr_;
	Address peerAddr_;


	ConnectionCallback connectioncallback_ = [](ConnectionPtr ptr) {};
	MessageCallback messagecallback_ = [](ConnectionPtr ptr, Buffer& buf) { buf.reset(); };
	WriteCompleteCallback writeCompleteCallback_ = [](ConnectionPtr ptr) {};
	CloseCallback closecallback_ = [](ConnectionPtr ptr) { ptr->close(); };

	Buffer inputBuffer_;       // handleRead
	Buffer outputBuffer_;      // handleWrite
	
	std::any context_;
};

END_NAMESPACE