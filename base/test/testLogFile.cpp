#include "../LogFile.h"
#include "../Thread.h"
#include "string"

USE_NAMESPACE

LogFile file("../../../Log/testLogFile.txt", 100);

void write_t1() {
	int n = 80;
	
	for(int i=0; i<n; ++i) {
		std::string s = "Thread 1 : write " + std::to_string(i) + " times\n";
		file.write(s.c_str(), s.size());
	}
}
int main() {
	
	Thread t1(write_t1);
	t1.start();
	
	for(int i=0; i<111; ++i) {
		std::string s = "Thread 2 : write " + std::to_string(i) + " times\n";
		file.write(s.c_str(), s.size());
	}

	t1.join();
	
	return 0;
}