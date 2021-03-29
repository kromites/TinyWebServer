#include "LogStream.h"

LogStream::OutputFunc LogStream::output_ = [](std::string&& val) { std::cout << val << std::endl;  };


LogStream::reference LogStream::operator<<(bool status) {
	buffer_.append(status ? "1" : "0", 1);
	return *this;
}

LogStream::reference LogStream::operator<<(short val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(unsigned short val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(int val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(unsigned int val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(long val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(unsigned long val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(long long val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(unsigned long long val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(float val) {
	return this->operator<<(static_cast<double>(val));
}

LogStream::reference LogStream::operator<<(double val) {
	formatToString(val);
	return *this;
}

LogStream::reference LogStream::operator<<(const char* val) {
	if (val)
		buffer_.append(val, strlen(val));
	else
		buffer_.append("(null)", 6);
	return *this;
}

LogStream::reference LogStream::operator<<(const unsigned char* val) {
	return this->operator<<(reinterpret_cast<const char*>(val));
}

LogStream::reference LogStream::operator<<(const std::string& val) {
	buffer_.append(val.c_str(), val.size());
	return *this;
}


LogStream::reference LogStream::operator<<(const LogBuffer<smallBufferSize>& v) {
	buffer_.append(v.data(), v.length());
	return *this;
}


LogStream::reference LogStream::operator<<(char val) {
	buffer_.append(&val, 1);
	return *this;
}

template <typename T>
void LogStream::formatToString(T t) {
	const std::string s = std::to_string(t);
	// snprintf(buffer_.data() + buffer_.length(), "%d", t);
	buffer_.append(s.c_str(), s.size());
}

