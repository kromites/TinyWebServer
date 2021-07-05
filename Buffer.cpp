#include "Buffer.h"
#include <cassert>
#include <cstring>

USE_NAMESPACE

Buffer::Buffer(): buffer_(InitialSize),readIndex_(0),writeIndex_(0)
{
	
}

void Buffer::reset() {
	writeIndex_ = readIndex_ = 0;
}

bool Buffer::empty() {
	return writeIndex_ == readIndex_;
}

void Buffer::consume(size_t len) {
	if (ReadableSize() > len) {
		readSize(len);
	}
	else {
		reset();
	}
}

void Buffer::write(const char* str, size_t len) {
	checkCapacity(len);
	memmove(writeStart(), str, len);
	writeSize(len);
}

void Buffer::writeSize(int len) {
	writeIndex_ += len;
}

void Buffer::readSize(int len) {
	readIndex_ += len;
}


char* Buffer::writeStart() {
	return buffer_.data() + writeIndex_;
}

char* Buffer::readString() {
	return buffer_.data() + readIndex_;
}

std::string Buffer::read(size_t len) {
	auto msg = std::string(readString(), len > ReadableSize() ? ReadableSize() : len);
	consume(len);
	return msg;
}

std::string Buffer::readAll() {
	return this->read(this->ReadableSize());
}

size_t Buffer::size() {
	return buffer_.size();
}

size_t Buffer::ReadableSize() {
	assert(readIndex_ <= writeIndex_);
	return writeIndex_ - readIndex_;
}

size_t Buffer::WritableSize() {
	return buffer_.size()- writeIndex_;
}


void Buffer::checkCapacity(int len) {
	if(WritableSize() < len) {
		buffer_.reserve(buffer_.size() * 2);
	}
}

