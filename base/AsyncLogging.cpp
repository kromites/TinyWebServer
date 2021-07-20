#include "AsyncLogging.h"

#include <cassert>

#include "LogFile.h"
#include "Timestamp.h"

USE_NAMESPACE

AsyncLogging::AsyncLogging(std::string& basename, int flushInterval)
	: flushInterval_(flushInterval),
	basename_(basename),
	running_(false),
	condition_(mutex_),
	latch_(1),
	thread_([this]() { this->threadFunc(); }, "Logging"),
	currentBuffer_(new largeBuffer),
	nextBuffer_(new largeBuffer)
{
	assert(basename_.size() > 1);
	currentBuffer_->bzero();
	nextBuffer_->bzero();
	buffers_.reserve(16);
}

AsyncLogging::~AsyncLogging() {
	if(running_) {
		stop();
	}
}

void AsyncLogging::start() {
	running_ = true;

	// thread start
	thread_.start();
	// make sure log thread start when start() returns
	latch_.wait();
}

void AsyncLogging::stop() {
	running_ = false;
	condition_.notify();
	thread_.join();
}

void AsyncLogging::setFileName(std::string& name) {
	basename_ = name;
}

void AsyncLogging::write(const char* logline, size_t len) {
	// latch_.countDown();
	// std::cout << "before mutex\n";
	MutexLockGuard guard(mutex_);
	// std::cout << "after mutex\n";
	// if sizeof buffer > len then append
	if(currentBuffer_->avail() > len) {
		// std::cout << 1 << std::endl;
		currentBuffer_->append(logline, len);
	}
	// else if buffer is full, push it and find the next spare buffer
	else {
		std::cout << 2 << std::endl;
		buffers_.push_back(std::move(currentBuffer_));
		if(nextBuffer_) {
			currentBuffer_ = std::move(nextBuffer_);
		}else { // two buffer both has used, then allocate a new buffer
			currentBuffer_.reset(new largeBuffer);
		}
		currentBuffer_->append(logline, len);

		// notify the back thread, a buffer is ready to write to the back.
		condition_.notify();
	}
}

void AsyncLogging::threadFunc() {
	assert(running_ == true);
	latch_.countDown();
	LogFile logFile(basename_, flushInterval_);
	BufferPtr newBuffer1(new largeBuffer);
	BufferPtr newBuffer2(new largeBuffer);
	newBuffer1->bzero();
	newBuffer2->bzero();
	BufferVec buffersToWrite;
	buffersToWrite.reserve(16);
	while(running_) {
		assert(newBuffer1 && newBuffer1->length() == 0);
		assert(newBuffer2 && newBuffer2->length() == 0);
		assert(buffersToWrite.empty());
		// swap out what need to be written, keep cs short
		// critical section
		{
			MutexLockGuard guard(mutex_);
			// must use if, if use while, it would be blocked here.
			if (buffers_.empty()) {
				condition_.wait();
			}
			buffers_.push_back(std::move(currentBuffer_));
			currentBuffer_ = std::move(newBuffer1);
			buffersToWrite.swap(buffers_);
			if (nextBuffer_ == nullptr) {
				nextBuffer_ = std::move(newBuffer2);
			}
		}
		assert(!buffersToWrite.empty());

		if(buffersToWrite.size() > 10) {
			
			buffersToWrite.erase(buffersToWrite.begin() + 2, buffersToWrite.end());
		}
		
		// output buffersToWrite to file
		for(auto& ptr : buffersToWrite) {
			logFile.write(ptr->data(), ptr->length());
		}
		// re-fill newBuffer1 and newBuffer 2

		if(buffersToWrite.size() > 2) {
			buffersToWrite.resize(2);
		}

		if(newBuffer1 == nullptr) {
			assert(!buffersToWrite.empty());
			newBuffer1 = std::move(buffersToWrite.back());
			buffersToWrite.pop_back();
			newBuffer1.reset();
		}

		if(newBuffer2 == nullptr) {
			assert(!buffersToWrite.empty());
			newBuffer2 = std::move(buffersToWrite.back());
			buffersToWrite.pop_back();
			newBuffer2.reset();
		}

		// clear
		buffersToWrite.clear();
		logFile.flush();
		
	}
	// flush output
	logFile.flush();
}

