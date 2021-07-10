#include "Logger.h"
#include "CurrentThread.h"
#include "AsyncLogging.h"
#include <time.h>
#include <string.h>

START_NAMESPACE

	__thread char t_errnoBuf[512];
	__thread char currentTime[32];
	__thread time_t t_lastSecond;

	const char* strerror_tl(int savedErrno) {
		snprintf(t_errnoBuf, sizeof t_errnoBuf, "%s", strerror(savedErrno));
		return t_errnoBuf;
	}

	// set class T(const char* str, unsigned len) for write str to buffer
	class T {
	public:
		T(const char* str, unsigned len) : str_(str), len_(len) {
		}

		const char* str_;
		const unsigned len_;
	};

	inline LogStream& operator<<(LogStream& s, T v) {
		s.append(v.str_, v.len_);
	}

	inline LogStream& operator<<(LogStream& s, Logger::SourceFile& file) {
		s.append(file.data_, file.size_);
		return s;
	}

	Logger::LogLevel g_logLevel = Logger::LogLevel::TRACE;

	void defaultOutput(const char* msg, size_t len) {
		size_t n = fwrite(msg, 1, len, stdout);
		(void)n;
	}

	//
	void defaultFlush() {
		fflush(stdout);
	}

	Logger::OutputFunc g_output = defaultOutput;
	Logger::FlushFunc g_flush = defaultFlush;

END_NAMESPACE

USE_NAMESPACE

Logger::MyLogger::MyLogger(LogLevel level, int savedErrno, SourceFile& file, int line) :
	time_(Timestamp::now()), level_(level), file_(file), line_(line) {
	stream_ << "Now Time: ";
	formateTime();
	stream_ << " tid = " << CurrentThread::tid();
	stream_ << "  " << levelToString(level) << " ";
	// except unblock error
	if (savedErrno != 11 || savedErrno != 0) {
		stream_ << " (errno: " << savedErrno << " " << strerror_tl(savedErrno) << " ) ";
	}
}

void Logger::MyLogger::finish() {
	stream_ << " -----> " << file_ << " : " << line_ << "\n";
}

void Logger::MyLogger::formateTime() {
	const auto times = time_.microSecondsSinceEpoch();
	const auto seconds = times / kMicroSecondsPerSecond;

	// only seconds don't equal lastSecond, update curTime;
	if (seconds != t_lastSecond) {
		const auto time = time_.toFormattedString();
		memcpy(currentTime, time.c_str(), 19);
	}

	char microSeconds[8];
	microSeconds[0] = '.';
	snprintf(microSeconds + 1, sizeof microSeconds - 1, "%06d", static_cast<int>(times % kMicroSecondsPerSecond));
	microSeconds[7] = ' ';
	stream_ << T(currentTime, 19) << T(microSeconds, 8);
}


Logger::Logger(LogLevel level, SourceFile file, int line): myLogger_(level, errno, file, line) {}

Logger::Logger(LogLevel level, SourceFile file, int line, const char* func): myLogger_(level, 0, file, line) {
	myLogger_.stream_ << " in " << func << " function ";
}

Logger::Logger(SourceFile file, int line, bool toAbort): myLogger_(toAbort ? LogLevel::FATAL : LogLevel::ERROR, errno,
                                                                   file, line) {
}

Logger::~Logger() {
	myLogger_.finish();
	if (myLogger_.level_ == LogLevel::FATAL) {
		const auto& buf = stream().buffer();
		g_output(buf.data(), buf.length());
		g_flush();
		abort();
	}
}


void Logger::setLogLevel(LogLevel level) {
	g_logLevel = level;
}

void Logger::setOutput(OutputFunc out) {
	g_output = out;
}

void Logger::setFlush(FlushFunc flush) {
	g_flush = flush;
}
