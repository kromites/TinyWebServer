#pragma once

#include "Address.h"


int tcpSocket();

void Bind(int, Address);

void Listen(int , int);

void setNonBlock(int);

int Accept(int sockfd, Address& addr);

int eventFd();

void Close(int sockfd);