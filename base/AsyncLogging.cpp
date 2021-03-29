#include "AsyncLogging.h"


void AsyncLogging::append(const char* logline, int len) {
	mutex_.lock();
	// todo if sizeof buffer > len then append
	if(curBuffer_->avail)
	// todp if buffer is full, push it and find the next spare buffer
}
