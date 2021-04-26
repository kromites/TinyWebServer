#pragma once

#include <cassert>

#include "Mutex.h"
#include "Condition.h"

#include <queue>

template<typename T>
class BlockingQueue {
public:
	BlockingQueue() : condition_(mutex_) {}

	T dequeue() {
		MutexLockGuard lock(mutex_);
		while(queue_.empty()) {
			condition_.wait();
		}
		assert(!queue_.empty());
		auto top = queue_.front();
		queue_.pop_front();
		return top;
	}

	void enqueue(T x) {
		MutexLockGuard lock(mutex_);
		queue_.push_back(x);
		condition_.notify();
	}

	size_t size() const {
		MutexLockGuard lock(mutex_);
		return queue_.size();
	}

private:
	MutexLock mutex_;
	Condition condition_;
	std::queue<T> queue_;
};