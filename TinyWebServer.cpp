#include <csignal>

#include "http/HttpServer.h"

#include "base/Util.h"
#include "base/AsyncLogging.h"

#include "EventLoop.h"
#include "Connection.h"

#include <memory>
#include <string>
#include <getopt.h>


USE_NAMESPACE

static std::string mainPage;
AsyncLogging* g_asyncLog = nullptr;

void asyncOutput(const char* msg, size_t len) {
	g_asyncLog->write(msg, len);
}

int main(int argc, char* argv[]) {

	// fixed : in order to prevent SIGPIPE in write, just ignore SIGPIPE
	::signal(SIGPIPE, SIG_IGN);

	// set param
	int port = 23456;
	int threadNum = 1;
	std::string basename = "../../../Log/WebServer.log";
	std::string curDirectory = "../../../Log";
	int opt;
	const char* str = "t:l:p:";
	while((opt = getopt(argc, argv, str)) != -1) {
		switch (opt) {
			case 't': {
				threadNum = atoi(optarg);
				break;
			}
			case 'p': {
				port = atoi(optarg);
				break;
			}
			case 'l': {
				curDirectory += atoi(optarg);
				if(curDirectory.size() < 14 || optarg[0] != '/') {
					printf("logPath should start with \"/\", so we will use the default log : LOG/WebServer.log\n");
					break;
				}
				basename = curDirectory;
				break;
			}
			default:
				break;
		}
	}

	// set log 
	AsyncLogging log(basename);
	log.start();
	g_asyncLog = &log;
	Logger::setLogLevel(Logger::LogLevel::TRACE);
	Logger::setOutput(asyncOutput);
	
	
	// set http server
	EventLoop loop;
	HttpServer server(&loop, port, threadNum);

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
