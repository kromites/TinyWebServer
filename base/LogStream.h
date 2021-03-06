#pragma once

/* class Buffer
 * in this head file, we should build a buffer class which is used in the log.
 * first, the buffer is built by char ptr, and the ptr manage this memory. so we can use a char ptr variable and
 * a buffer ptr to manage it.
 * second, we should check the function of class.
 *		-> constructor, default and copy
 *		-> destructor = default;
 *		-> operator override: =, +=
 *		-> feature function:
*/
#include "Util.h"

#include <functional>
#include <iostream>
#include <stddef.h>
#include <string>
#include <cstring>


START_NAMESPACE

constexpr int smallBufferSize = 4000;
constexpr int largeBufferSize = 4000 * 1000;

// we should use Buffer<size> to create a buffer, so we can use template.

template<size_t SIZE>
class LogBuffer {                                
public:
	LogBuffer(): data_{}, current_(data_){}
	LogBuffer(const LogBuffer& buffer):data_{}, current_(data_ + buffer.length()) {
		memcpy(this->data_, buffer.data(), buffer.length());
	}
	
	LogBuffer& operator=(const LogBuffer& buffer) {
		if (this == buffer)
			return *this;
		memcpy(this->data_, buffer.data(), buffer.length());
		current_ = data_ + buffer.length();
		return *this;
	}

	~LogBuffer() = default;

	void append(const char* str) {
		append(str, strlen(str));
	}

	void append(const char* str, int len) {
		if (this->avail() > len) {
			memcpy(current_, str, len);
			current_ += len;
		}
	}

	const char* data() const {
		return data_;
	}

	size_t avail() const {
		return (data_ + sizeof data_) - current_;
	}
	
	size_t length() const {
		return current_- data_;
	}

	void reset() {
		current_ = data_;
	}

	void bzero() {
		memset(data_, 0, sizeof data_);
	}

	const char* current() { return current_; }


private:
	// define variable.
	char data_[SIZE];
	char* current_;

};


/* class LogStream
 * in this class, we can use the Buffer class as the buffer of log stream.
 * 
 */
class LogStream {
public:
	using OutputFunc = std::function<void(std::string&&)>;
	using reference = LogStream&;
	using SmallBuffer = LogBuffer<smallBufferSize>;
public:
	

	void append(const char* str, int len) {
		buffer_.append(str, len);
	}
	
	reference operator<<(bool);
	reference operator<<(short);
	reference operator<<(unsigned short);
	reference operator<<(int);
	reference operator<<(unsigned int);
	reference operator<<(long);
	reference operator<<(unsigned long);
	reference operator<<(long long);
	reference operator<<(unsigned long long);
	reference operator<<(float);
	reference operator<<(double);
	reference operator<<(char);
	reference operator<<(const char*);
	reference operator<<(const unsigned char*);
	reference operator<<(const std::string&);
	reference operator<<(const SmallBuffer&);


	const SmallBuffer& buffer() const { return buffer_; }
	

private:
	static OutputFunc output_;
	SmallBuffer buffer_;

	template<typename T>
	void formatToString(T);
};

END_NAMESPACE