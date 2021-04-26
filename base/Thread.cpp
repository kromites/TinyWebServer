#include <cassert>

#include "Thread.h"
#include "Logger.h"
#include "CurrentThread.h"
class MainThreadInitializer {
public:
	MainThreadInitializer() {
		CurrentThread::t_threadName = "main";
		CurrentThread::tid();
	}
};

MainThreadInitializer init;


struct ThreadData {
	ThreadData(Thread::ThreadFunc func, CountDownLacth* latch, const std::string name, pid_t* tid):
		func_(func), lacth_(latch), name_(name), tid_(tid) {}

	void runInThread(){
		*tid_ = CurrentThread::tid();
		tid_ = nullptr;
		lacth_->countDown();
		lacth_ = nullptr;
		CurrentThread::t_threadName = name_.c_str();

		try {
			func_();
			CurrentThread::t_threadName = "finished";
		}
		catch (const std::exception& e) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			fprintf(stderr, "exception: %s\n", e.what());
			abort();
		}
		catch (...) {
			CurrentThread::t_threadName = "crashed";
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			throw;
		}
		
	}
	
	Thread::ThreadFunc func_;
	CountDownLacth* lacth_;
	std::string name_;
	pid_t* tid_;
};

void* runThread(void* data) {
	auto* ptr = static_cast<ThreadData*>(data);
	ptr->runInThread();
	ptr = nullptr;
	delete ptr;
	return nullptr;
}

Thread::Thread(ThreadFunc func, const std::string& name) :
	func_(func), name_(name), latch_(1) {
	++numCreated_;
	if(name.empty()) {
		setDefaultName();
	}
}

Thread::~Thread() {
	if(!started_ && !joined_) {
		pthread_detach(pthreadId_);
	}
}

void Thread::start() {
	started_ = true;
	auto* args = new ThreadData(func_, &latch_, name_, &tid_);
	if (pthread_create(&pthreadId_, nullptr, runThread, static_cast<void*>(args)) == -1) {
		delete args;
		started_ = false;
		LOG_ERROR << "Failed in pthread_create";
	}else {
		latch_.wait();
		assert(tid_ > 0);
	}
	
	
}

int Thread::join() {
	assert(started_);
	assert(!joined_);
	joined_ = true;
	return pthread_join(pthreadId_, nullptr);
}

bool Thread::isStarted() {
	return started_;
}

pid_t Thread::getTid() const {
	return tid_;
}

const std::string& Thread::name() const{
	return name_;
}


int Thread::numCreated() {
	return numCreated_;
}


void Thread::setDefaultName() {
	name_ = "Thread " + std::to_string(numCreated_);
}

std::atomic_int32_t Thread::numCreated_;