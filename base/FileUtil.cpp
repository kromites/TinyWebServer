#include <cstdio>
#include "FileUtil.h"

USE_NAMESPACE

MyFile::MyFile(const std::string& name): file_(fopen(name.c_str(), "ae")), writeBytes_(0){
	setbuffer(file_, buffer_, sizeof buffer_);
}

MyFile::~MyFile() {
	fclose(file_);
}

void MyFile::write(const char* str, size_t len) {
	auto n = fwrite_unlocked(str, 1, len, file_);
	auto rest = len - n;
	while(rest > 0) {
		size_t x = fwrite_unlocked(str + n, 1, rest, file_);
		if (x == 0) {
			const auto err = ferror(file_);
			if (err) {
				fprintf(stderr, "MyFile::write() failed !\n");
			}
			break;
		}
		n += x;
		rest = len - n;
	}

	writeBytes_ += n;
}

void MyFile::flush() const{
	fflush(file_);
}

