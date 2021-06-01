#include "Logger.h"
#include "CurrentThread.h"

#include <time.h>
#include <string.h>


inline LogStream& operator<<(LogStream& s, Logger::SourceFile& file) {
	s.append(file.data_, file.size_);
	return s;
}
//
// Logger::Logger(LogLevel level, const char* file, int line): file_(file) {
// 	stream_ << levelToString(level) << " " << file_<< " in line " << line << ":  ";
// }

Logger::LogLevel g_logLevel = Logger::LogLevel::TRACE;

void Logger::setLogLevel(LogLevel level) {
	g_logLevel = level;
}

Logger::MyLogger::MyLogger(LogLevel level, int savedErrno, SourceFile& file, int line) :
	time_(Timestamp::now()),level_(level), file_(file), line_(line) {
	// stream_.append(t, strlen(t)-1);
	stream_ << "Now Time: " << time_.toFormattedString();
	stream_ << " tid = " << CurrentThread::tid();
	stream_ << "  "  << levelToString(level) << " " ;
	if(level >= LogLevel::ERROR) {
		stream_ << " (errno: " << savedErrno << " " <<strerror(savedErrno) << " ) ";
	}
}

void Logger::MyLogger::finish() {
	stream_ << " -----> " << file_ << " : " << line_ << "\n";
}

// void defaultOutput(const char* msg, size_t len) {
// 	size_t n = fwrite(msg, 1, len, stdout);
// }
//
// void defaultFlush() {
// 	fflush(stdout);
// }
//
// Logger::OutputFunc g_output = defaultOutput;
// Logger::FlushFunc g_flush = defaultFlush;

Logger::Logger(LogLevel level, SourceFile file, int line):myLogger_(level, errno, file, line) {
	
}

Logger::Logger(LogLevel level, SourceFile file, int line,  const char* func): myLogger_(level, 0, file, line) {
	myLogger_.stream_ << " in " << func << " funtion ";
}

Logger::Logger(SourceFile file, int line, bool toAbort): myLogger_(toAbort? LogLevel::FATAL : LogLevel::ERROR, errno, file, line) {
	
}

Logger::~Logger() {
	// myLogger_.stream_ << "\t Now Time: " << GetCurTime();
	myLogger_.finish();
	
	if(myLogger_.level_ == LogLevel::FATAL) {
		// g_flush();
		abort();
	}
}

