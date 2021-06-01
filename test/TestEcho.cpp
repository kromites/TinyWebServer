#include "../base/Logger.h"

#include "../EventLoop.h"
#include "../TcpServer.h"
#include "../Connection.h"

#include <memory>

// void threadInit(EventLoop* loop) {
// 	LOG_INFO << "io thread init";
// 	loop->assertInLoopThread();
// }

int main() {
	// set log level
	Logger::setLogLevel(Logger::LogLevel::TRACE);

	EventLoop loop;
	TcpServer server(&loop, 23457, 1);

	server.setConnectionCallback([](std::shared_ptr<Connection> conn) {
		if (conn->connected()) {
			LOG_INFO << "EchoServer - " << conn->peerAddr().toString() << " -> "
				<< conn->hostAddr().toString() << " is "
				<< (conn->connected() ? "UP" : "DOWN");
		}
		});

	server.setMessageCallback([](std::shared_ptr<Connection> conn, Buffer& buf) {
		std::string msg(buf.readAll());
		LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, ";
		conn->send(msg);
		});

	LOG_INFO << "server starts";

	server.start();
	
}