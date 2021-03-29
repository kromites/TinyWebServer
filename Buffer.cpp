#include "Buffer.h"
#include <cassert>
#include <cstring>

Buffer::Buffer(): buffer_(InitialSize),readIndex_(0),writeIndex_(0)
{
	
}

void Buffer::reset() {
	writeIndex_ = 0;
	readIndex_ = 0;
}

bool Buffer::empty() {
	if (readIndex_ == writeIndex_) {
		reset();
		return true;
	}
	return false;
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

