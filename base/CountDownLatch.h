#pragma once

#include "Mutex.h"
#include "Condition.h"

#include "Util.h"

START_NAMESPACE

class CountDownLacth {
public:
	explicit CountDownLacth(int count);

	void wait();     /* main thread wait work threads init */

	void countDown();  /* work threads init until all work threads have init then call the main thread */

	int getCount() const;
private:
	mutable MutexLock mutex_;
	Condition condition_;
	int count_;
};

END_NAMESPACE