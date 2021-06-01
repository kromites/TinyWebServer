#pragma once

#include <vector>
#include <string>

constexpr int InitialSize = 1024;

class Buffer {
public:
	Buffer();

	~Buffer() = default;

	bool empty();
	void reset();
	void consume(size_t len);
	
	void write(const char* str, size_t len);
	// todo read;
	
	void writeSize(int);
	void readSize(int);
	
	char* writeStart();
	char* readString();

	std::string read(size_t len);
	std::string readAll();
	
	size_t size();
	size_t ReadableSize();
	size_t WritableSize();

	// size_t readFd(int fd, int* savedErrno);
	
private:
	void checkCapacity(int len);


	std::vector<char> buffer_;
	size_t readIndex_;
	size_t writeIndex_;
};