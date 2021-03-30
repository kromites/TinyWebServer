#include "Mutex.h"
#include "CurrentThread.h"
#include <cassert>

MutexLock::MutexLock(): holder_(0) {
	pthread_mutex_init(&mutex_, nullptr);
}

MutexLock::~MutexLock() {
	assert(holder_ == 0);
	pthread_mutex_destroy(&mutex_);
}

bool MutexLock::isLockedByThisThread() {
	return holder_ == CurrentThread::tid();      // todo make CurrentThread .{cpp,h}
}

void MutexLock::assertLocked() {
	assert(isLockedByThisThread());
}

void MutexLock::lock() {
	pthread_mutex_lock(&mutex_);
	holder_ = CurrentThread::tid();
}

void MutexLock::unlock() {
	holder_ = 0;
	pthread_mutex_unlock(&mutex_);
}

auto MutexLock::getPthreadMutex() {
	return &mutex_;
}

MutexLockGuard::MutexLockGuard(MutexLock& mutex): mutex_(mutex) {
	mutex_.lock();
}

MutexLockGuard::~MutexLockGuard() {
	mutex_.unlock();
}

