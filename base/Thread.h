#pragma once

#include "Util.h"

#include <pthread.h>
#include <functional>
#include <atomic>

#include "CountDownLatch.h"
START_NAMESPACE

class Thread {
public:
	typedef std::function<void()> ThreadFunc;

	explicit Thread(ThreadFunc, const std::string& name = std::string());
	~Thread();

	void start();
	int join();
	bool isStarted();
	pid_t getTid() const;
	const std::string& name() const;

	static int numCreated();
private:
	void setDefaultName();
	
	bool started_;
	bool joined_;
	pthread_t pthreadId_;
	pid_t tid_;

	ThreadFunc func_;
	std::string name_;

	CountDownLacth latch_;

	static std::atomic_int32_t numCreated_;
};

END_NAMESPACE