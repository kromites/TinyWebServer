#include "CountDownLatch.h"

USE_NAMESPACE

CountDownLacth::CountDownLacth(int count): condition_(mutex_), count_(count) {
	
}

void CountDownLacth::wait() {
	MutexLockGuard guard(mutex_);
	while(count_ > 0) {
		condition_.wait();
	}
}

void CountDownLacth::countDown() {
	MutexLockGuard guard(mutex_);
	--count_;
	if(count_ == 0) {
		condition_.notifyAll();
	}
}

int CountDownLacth::getCount() const {
	MutexLockGuard guard(mutex_);
	return count_;
}


		