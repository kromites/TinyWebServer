#pragma once

#include "Mutex.h"

#include "Util.h"

START_NAMESPACE

class Condition {
public:
	Condition(MutexLock& mutex);

	Condition() = delete;
	Condition(const Condition&) = delete;
	Condition& operator=(const Condition&) = delete;

	~Condition();

	void wait();

	void notify();

	void notifyAll();


private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

END_NAMESPACE