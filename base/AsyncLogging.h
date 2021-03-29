#include <mutex>
#include <memory>


class AsyncLogging {
private:
	std::mutex mutex_;
public:
	void append(const char* logline, int len);
};