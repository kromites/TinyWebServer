#pragma once

#include <memory>
#include "Util.h"
#include "Mutex.h"
#include "FileUtil.h"

START_NAMESPACE

class LogFile {
public:
	LogFile(const std::string& basename, int flushEveryN = 1024);
	~LogFile() = default;

	void write(const char* str, int len);
	void flush();
	
	
private:
	void write_unlocked(const char* str, int len);
	
	const std::string basename_;
	// append to the buffer every N times the file will be written and the buffer will be flushed.
	const int flushEveryN_;
	int count_;
	MutexLock mutex_;
	std::unique_ptr<MyFile> myfile_;
};


END_NAMESPACE