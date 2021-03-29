#pragma once

#include "./base/Logger.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <string>


class Address {
public:
	Address() = default;
	Address(const std::string& ip, uint16_t port);
	Address(uint32_t ip, uint16_t port);

	// std::string toString() const;

	static Address getListenAddress(uint16_t port);

	sockaddr* getAddr() {
		return reinterpret_cast<sockaddr*>(&sockaddr_);
	}
	size_t size() {
		 return sizeof(sockaddr_);
	}

	in_addr getIPAddr() {
		return sockaddr_.sin_addr;
	}

	uint16_t getTcpPort() {
		return sockaddr_.sin_port;
	}

	std::string toString() const;


private:
	friend std::pair<int, Address> Accept(int fd);
	sockaddr_in sockaddr_;
};






