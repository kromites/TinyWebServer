#include <unistd.h>

#include "../Address.h"
#include "../Socket.h"

int main() {
	auto listenAddr = Address::getListenAddress(12345);
	auto listenfd = tcpSocket();
	Bind(listenfd, listenAddr);
	Listen(listenfd, 10);
	
	::sleep(30);
	
	
}