#include <unistd.h>

#include "../base/Logger.h"
#include "../EventLoop.h"
#include "../Acceptor.h"
#include "../Address.h"
#include "../Poller.h"
#include "../Socket.h"



int main() {
	LOG_DEBUG << "main(): pid = " << getpid();

	EventLoop loop;
	Acceptor acceptor(&loop, 23456);
	std::function<void(int)> newConnection = [&acceptor](int sockfd) {
		LOG_DEBUG << "newConnection(): accepted a new connection from  " << acceptor.peerAddress.toString();
		write(sockfd, "how are you?\n", 13);
		// Close(sockfd);
	};
	acceptor.setFuncPtr(newConnection);
	acceptor.listen(10);

	LOG_DEBUG << "start loop";
	loop.loop();
	
}