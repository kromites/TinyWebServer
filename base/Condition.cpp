#include "Condition.h"

USE_NAMESPACE

Condition::Condition(MutexLock& mutex):mutex_(mutex) {
	pthread_cond_init(&pcond_, nullptr);
}

Condition::~Condition() {
	pthread_cond_destroy(&pcond_);
}

void Condition::wait() {
	pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
}

void Condition::notify() {
	pthread_cond_signal(&pcond_);
}

void Condition::notifyAll() {
	pthread_cond_broadcast(&pcond_);
}




