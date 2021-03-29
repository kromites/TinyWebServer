#pragma once

#include "Address.h"


int tcpSocket();

void Bind(int, Address);

void Listen(int , int);

void setNonBlock(int);

std::pair<int, Address> Accept(int);

