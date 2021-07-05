#include "CurrentThread.h"

#include <cstdio>
#include <unistd.h>
#include <sys/syscall.h>

USE_NAMESPACE

__thread int CurrentThread::t_cachedTid = 0;
__thread char CurrentThread::t_tidString[32];
__thread size_t CurrentThread::t_tidStringLength;
__thread const char* CurrentThread::t_threadName;

void CurrentThread::cacheTid() {
	t_cachedTid = syscall(SYS_gettid);
	t_tidStringLength = sprintf(t_tidString, "%6d", t_cachedTid);
}


int CurrentThread::tid() {
	if(t_cachedTid == 0) {
		cacheTid();
	}
	return t_cachedTid;
}


const char* CurrentThread::tidString() {
	if(t_cachedTid == 0) {
		cacheTid();
	}
	return t_tidString;
}


int CurrentThread::tidStringLength() {
	if (t_tidStringLength == 0) {
		cacheTid();
	}
	return t_tidStringLength;
}

const char* CurrentThread::threadName() {
	if(t_threadName == nullptr) {
		cacheTid();
	}
	return t_threadName;
}



	