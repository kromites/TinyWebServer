#include "../Acceptor.h"

std::function<void(int)> acceptSet;
std::function<void(int)> acceptSet2;

int main() {
	Logger::setLogLevel(Logger::LogLevel::TRACE);


	acceptSet = [](int fd) {
		LOG_TRACE << "connfd = " << fd;
	};

	Acceptor acceptor(23457, acceptSet);

	acceptor.onAccept();
	auto ret = acceptor.getListenFd();
	LOG_TRACE << "listenfd = " << ret;

	acceptSet2 = [](int fd) {
		LOG_TRACE << "listenfd = " << fd;
	};
	acceptor.setFuncPtr(acceptSet2);
	acceptor.onAccept();
}
