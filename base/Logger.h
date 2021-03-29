#pragma once

#include "LogStream.h"

#include <cstring>

// class TimeStamp {
// 	static int cnt;
// 	TimeStamp() {
// 		++cnt;
// 	}
// };

// int TimeStamp::cnt = 0;


//todo complete ctr and log_level.
//todo implement class timestamp.
//todo handle the error and fatal, we can print the reason.
//todo implement the class MyLogger.




class Logger {
public:
	enum class LogLevel {
		TRACE,
		DEBUG,
		INFO,
		WARN,
		ERROR,
		FATAL,
		NUM_LOG_LEVELS,
	};

	static LogLevel level_;
	// try to get the file name without path name.
	class SourceFile {
	public:
		template<size_t N>
		SourceFile(const char (&filename)[N]): data_(filename), size_(N-1) {
			const char* slash = strrchr(data_, '/');
			if(slash) {
				data_ = slash + 1;
				size_ = size_ - (data_ - filename);
			}
		}
	
		explicit SourceFile(const char* filename): data_(filename) {
			const char* slash = strrchr(data_, '/');
			if(slash) {
				data_ = slash + 1;
			}
			size_ = static_cast<size_t>(strlen(data_));
		}
		
		friend LogStream& operator<<(LogStream& s, SourceFile& file);
	private:
		const char* data_;
		size_t size_;
	};
	
	// Logger(LogLevel level, const char* file, int line);
	Logger(LogLevel level, SourceFile file, int line);
	Logger(LogLevel level, SourceFile file, int line,  const char* func);
	Logger(SourceFile file, int line, bool toAbort);
	~Logger();

	LogStream& stream() {
		return myLogger_.stream_;
	}

	static LogLevel logLevel();
	static void setLogLevel(LogLevel level);

	// using OutputFunc = void(*)(const char*, size_t);
	// using FlushFunc = void(*)();
	//
	// static void setOutput(OutputFunc);
	// static void setFlush(FlushFunc);
	//
	// static LogLevel logLevel();
	// static void setLogLevel(LogLevel level);

	// output and flush?


private:
	class MyLogger {
	public:
		MyLogger(LogLevel level, int savedErrno, SourceFile& file, int line);
	
		void finish();
		
		LogLevel level_;
		SourceFile file_;
		int line_;
	
		LogStream stream_;
		
	};

	static std::string levelToString(LogLevel level) {
		switch (level) {  // NOLINT(clang-diagnostic-switch-enum)
		case LogLevel::DEBUG:
			return "DEBUG";
		case LogLevel::ERROR:
			return "ERROR";
		case LogLevel::FATAL:
			return "FATAL";
		case LogLevel::INFO:
			return "INFO ";
		case LogLevel::TRACE:
			return "TRACE";
		case  LogLevel::WARN:
			return "WARN ";
		default:
			return "UNKNOWN";
		}
	}

	MyLogger myLogger_;
};


extern Logger::LogLevel g_logLevel;

inline Logger::LogLevel Logger::logLevel() {
	return g_logLevel;
}

static Logger::LogLevel defaultLogLevel = Logger::LogLevel::INFO;


#define LOG  if(Logger::logLevel() <= defaultLogLevel) Logger(defaultLogLevel, __FILE__, __LINE__).stream()
#define LOG_TRACE if(Logger::logLevel() <= Logger::LogLevel::TRACE) Logger(Logger::LogLevel::TRACE, __FILE__, __LINE__).stream()  // NOLINT(cppcoreguidelines-macro-usage)
#define LOG_DEBUG if(Logger::logLevel() <= Logger::LogLevel::DEBUG) Logger(Logger::LogLevel::DEBUG, __FILE__, __LINE__).stream()  // NOLINT(cppcoreguidelines-macro-usage)
#define LOG_INFO if(Logger::logLevel()<= Logger::LogLevel::INFO) Logger(Logger::LogLevel::INFO, __FILE__, __LINE__).stream()  // NOLINT(cppcoreguidelines-macro-usage)


#define LOG_WARN  Logger(Logger::LogLevel::WARN, __FILE__, __LINE__).stream()
#define LOG_ERROR  Logger(Logger::LogLevel::ERROR, __FILE__, __LINE__).stream()
#define LOG_FATAL  Logger(Logger::LogLevel::FATAL, __FILE__, __LINE__).stream()
// TODO handle with error and fatal