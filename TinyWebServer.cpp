// TinyWebServer.cpp: 定义应用程序的入口点。
//

#include "TinyWebServer.h"
#include "Poller.h"

#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <sys/epoll.h>
// what a f*ck world

using namespace std;

const std::string html = "<html>\n<head>\n <title>hello zw</title></head><body><p>hello world</p><p>this is a test </p></body></html>";

std::string httpResponse = "200 HTTP/1.1 OK\n\n";

const int Maxsize = 4096;
char buffer[Maxsize];

int main()
{
	httpResponse += html;
	sockaddr_in serveraddr;
	
	
	auto listenfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	bzero(&serveraddr, sizeof serveraddr);
	serveraddr.sin_port = htons(10087);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	auto ret = bind(listenfd, reinterpret_cast<sockaddr*> (&serveraddr), sizeof serveraddr);
	if(ret == -1) {
		perror("error_bind");
		exit(-1);
	}
	
	auto lis = listen(listenfd, 10); //10: max queue nums
	std::cout << lis << std::endl;


	Poller poller;
	auto epollfd = poller.get_epollfd();
	
	// auto epollfd = epoll_create1(0);
	//
	// todo need to check epollfd.
	// if(epollfd == -1) {
	// 	perror("error_create1");
	// 	exit(-1);
	// }
	
	epoll_event ev, events[10];
	ev.events = EPOLLIN;
	ev.data.fd = listenfd;

	poller.add_fd(ev);

	
	// if(epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &ev) == -1) {
	// 	perror("error_ctl");
	// 	exit(-1);
	// }
	
	auto num = 0;
	auto maxi = 0;
	while (true) {
		// 这些可以全部集成一个函数
		auto nfds = poller.get_numEvents(-1);
		// auto nfds = epoll_wait(epollfd, events, 10, -1);
		std::cout << nfds << std::endl;
		if(nfds == -1) {
			perror("error_wait");
			exit(-1);
		}

		for(auto i = 0; i < nfds; ++i) {
			if(events[i].data.fd == listenfd) {
				auto socketfd = accept(listenfd, nullptr, nullptr);
				std::cout << ++num <<" : " << socketfd << std::endl;
				
				if (socketfd > 0) {
					read(socketfd, buffer, Maxsize);
					write(socketfd, httpResponse.c_str(), httpResponse.size());
					shutdown(socketfd, SHUT_RDWR);
				}
			}
		}
		
	}
	
}
