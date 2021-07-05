/*
 *	This header sets the file standard operator for the logger file.
 *	funtion : fopen  ,  fwrite , fflush , fclose
 */


#pragma once

#include "Util.h"
#include <string>

START_NAMESPACE

class MyFile {
public:
	explicit MyFile(const std::string& name);
	~MyFile();

	void write(const char* str, size_t len);
	void flush() const;
	off_t writeBytes() const { return writeBytes_; }
private:
	std::string filename_;
	FILE* file_;
	off_t writeBytes_;
	char buffer_[64 * 1024];
};

END_NAMESPACE