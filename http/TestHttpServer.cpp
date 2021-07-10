#include <csignal>

#include "HttpServer.h"

#include "../base/Util.h"
#include "../base/Logger.h"

#include "../EventLoop.h"
#include "../Connection.h"

#include <memory>
#include <string>



#include "HttpParse.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
// #include "../base/AsyncLogging.h"

USE_NAMESPACE

static std::string mainPage;
// AsyncLogging* g_asyncLog = nullptr;
// void asyncOutput(const char* msg, size_t len)
// {
// 	g_asyncLog->write(msg, len);
// }

int main() {

	// fixed : in order to prevent SIGPIPE in write, just ignore SIGPIPE
	::signal(SIGPIPE, SIG_IGN);
	
	// set log 
	// std::string basename = "../../../Log/echo1.log";
	// AsyncLogging log(basename);
	// log.start();
	// g_asyncLog = &log;
	Logger::setLogLevel(Logger::LogLevel::WARN);
	// Logger::setOutput(asyncOutput);


	// set http server
	EventLoop loop;
	HttpServer server(&loop, 23456, 4);

	server.setConnectionCallback([&server](std::shared_ptr<Connection> conn) {
		LOG_DEBUG << "set connection callback";
		server.onConnection(conn);
		});
	

	mainPage = "<html><head><title>a light web server </title>"
		"<link rel=\"icon\" href=\"data:; base64, =\">"
		"</head>\n"
		"<body><h1>this is "
		"what you want</h1><p>"
		"Hello World"
		"</p></body></html>\n";

	LOG_DEBUG << "test ";

	server.setMessageCallback([&server](std::shared_ptr<Connection> conn, Buffer& buff) {
		server.onMessage(conn, buff, mainPage);
		});

	
	
	LOG_INFO << "server starts";
	server.start();

	return 0;
}

