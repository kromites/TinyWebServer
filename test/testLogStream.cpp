#include "../base/LogStream.h"
//
// #define LOG LogStream() 
#include <iostream>


int main() {
	LogStream() << "hello world" << 1 << false;
}