#include "LogFile.h"

USE_NAMESPACE

LogFile::LogFile(const std::string& basename, int flushEveryN)
	: basename_(basename),
	flushEveryN_(flushEveryN),
	count_(0),
	myfile_(new MyFile(basename))
{
}

void LogFile::write(const char* str, int len) {
	MutexLockGuard guard(mutex_);
	write_unlocked(str, len);
}

void LogFile::flush() {
	MutexLockGuard guard(mutex_);
	myfile_->flush();
}

void LogFile::write_unlocked(const char* str, int len) {
	myfile_->write(str, len);
	++count_;
	if(count_ >= flushEveryN_) {
		count_ = 0;
		myfile_->flush();
	}
}


