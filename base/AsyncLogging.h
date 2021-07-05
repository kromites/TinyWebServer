#pragma once

#include <memory>
#include <vector>

#include "Util.h"
#include "LogStream.h"
#include "Mutex.h"
#include "Condition.h"
#include "Thread.h"

#include "../Buffer.h"

START_NAMESPACE

class AsyncLogging {
public:
	AsyncLogging(std::string& basename, int flushInterval = 1024);
	~AsyncLogging();

	void start();

	void stop();

	void setFileName(std::string& name);
	
	void write(const char* logline, size_t len);
	void threadFunc();
private:
	using largeBuffer = LogBuffer<largeBufferSize>;
	using BufferPtr = std::unique_ptr<largeBuffer>;
	using BufferVec = std::vector<BufferPtr>;

	const int flushInterval_;
	
	std::string basename_;
	bool running_;

	MutexLock mutex_;
	Condition condition_;
	CountDownLacth latch_;
	Thread thread_;
	
	BufferPtr currentBuffer_;
	BufferPtr nextBuffer_;
	BufferVec buffers_;
};

END_NAMESPACE