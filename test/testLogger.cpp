
#include "../base/Logger.h"


int main() {
	// then add file name and line index
	// step two: add log level
	// step three: complete log level
	// add time stamp
	// add feature in log error	-> print error information
	//				print errno and explain
	// add feature : setLogLevel
	// add feature : print thread id in which thread
	LOG_INFO << "hello world";
	LOG << "this should be log info";
	Logger::level_ = Logger::LogLevel::TRACE;
	
	defaultLogLevel = Logger::LogLevel::DEBUG;
	LOG << "this should be log debug";
	defaultLogLevel = Logger::LogLevel::INFO;
	LOG << "this is log info";


	LOG_DEBUG << "this would be debug";
	LOG_TRACE << "this would be trace";

	// Logger(defaultLogLevel, __FILE__, __LINE__, __func__).stream() << "this would be function";
	//
	// Logger(__FILE__, __LINE__, false).stream() << "this would be error";
	// Logger(__FILE__, __LINE__, true).stream() << "this would be fatal";

	LOG_ERROR << "not only log_error, but also info";
}
