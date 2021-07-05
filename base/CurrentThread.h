#pragma once

#include "Util.h"

#include <cctype>
#include <cstdint>
#include <string>
#include <thread>

START_NAMESPACE

namespace CurrentThread{
	extern __thread int t_cachedTid;
	extern __thread char t_tidString[32];
	extern __thread size_t t_tidStringLength;
	extern __thread const char* t_threadName;

	void cacheTid();
	
	int tid();

	const char* tidString();

	int tidStringLength();

	const char* threadName();

	
}

END_NAMESPACE