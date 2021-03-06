#pragma once

#include <pthread.h>

#include "Util.h"

START_NAMESPACE

class MutexLock {
public:
	MutexLock();
	MutexLock(const MutexLock&) = delete;
	MutexLock& operator=(const MutexLock&) = delete;
	~MutexLock();

	bool isLockedByThisThread();
	
	void assertLocked();

	void lock();

	void unlock();

	pthread_mutex_t* getPthreadMutex();

private:
	pthread_mutex_t mutex_;
	pid_t holder_;
};


class MutexLockGuard {
public:
	explicit MutexLockGuard(MutexLock&);
	
	MutexLockGuard() = delete;
	MutexLockGuard(const MutexLockGuard&) = delete;
	MutexLockGuard& operator=(const MutexLockGuard&) = delete;

	~MutexLockGuard();
private:
	MutexLock& mutex_;
};

END_NAMESPACE