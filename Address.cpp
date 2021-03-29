#include "Address.h"
#include <arpa/inet.h>

Address::Address(const std::string& ip, uint16_t port) : Address(inet_addr(ip.c_str()), port) {

}

Address::Address(uint32_t ip, uint16_t port) {
	bzero(&sockaddr_, sizeof sockaddr_);
	sockaddr_.sin_port = ntohs(port);
	sockaddr_.sin_family = AF_INET;
	sockaddr_.sin_addr.s_addr = ip;
}


Address Address::getListenAddress(uint16_t port) {
	return Address(INADDR_ANY, port);
}

std::string Address::toString() const {
	std::string res;
	res += inet_ntoa(sockaddr_.sin_addr);
	res += " : ";
	res += std::to_string(ntohs(sockaddr_.sin_port));
	return res;
}


