#include "../base/Util.h"
#include "../base/Logger.h"
#include "../base/AsyncLogging.h"

#include "../EventLoop.h"
#include "../TcpServer.h"
#include "../Connection.h"

#include <memory>


USE_NAMESPACE

AsyncLogging* g_asyncLog = nullptr;

void asyncOutput(const char* msg, size_t len)
{
	g_asyncLog->write(msg, len);
}

int main() {

	std::string basename = "../../../Log/echo1.log";
	AsyncLogging log(basename);
	log.start();
	g_asyncLog = &log;

	// Logger::setLogLevel(Logger::LogLevel::DEBUG);
	Logger::setOutput(asyncOutput);
	int cnt = 0;
	const int kBatch = 100;

	// set log level

	EventLoop loop;
	TcpServer server(&loop, 23456, 1);

	server.setConnectionCallback([](std::shared_ptr<Connection> conn) {
		if (conn->connected()) {
			LOG_INFO << "EchoServer - " << conn->peerAddr().toString() << " -> "
				<< conn->hostAddr().toString() << " is "
				<< (conn->connected() ? "UP" : "DOWN");
		}
		});

	server.setMessageCallback([](std::shared_ptr<Connection> conn, Buffer& buf) {
		std::string msg(buf.readAll());
		LOG_INFO << "channel(fd : " <<conn->fd() << ") echo " << msg.size() << " bytes, ";
		conn->send(msg);
		});

	LOG_INFO << "server starts";

	server.start();

	return 0;
}
