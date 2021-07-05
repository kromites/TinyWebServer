#include "Timestamp.h"


#include <cinttypes>
#include <sys/time.h>

USE_NAMESPACE

Timestamp::Timestamp():microSecondsSinceEpoch_(0) {
	
}

Timestamp::Timestamp(int64_t microSecondsSinceEpochArg):microSecondsSinceEpoch_(microSecondsSinceEpochArg) {
	
}

// check it is useful?
void Timestamp::swap(Timestamp& that) {
	int64_t temp = that.microSecondsSinceEpoch_;
	that.microSecondsSinceEpoch_ = 
	this->microSecondsSinceEpoch_ = temp;
}

std::string Timestamp::toString() const {
	char buf[32] = {0};
	int64_t seconds = microSecondsSinceEpoch_ / kMicroSecondsPerSecond;
	int64_t microseconds = microSecondsSinceEpoch_ % kMicroSecondsPerSecond;
	snprintf(buf, sizeof(buf), "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
	return buf;
}

std::string Timestamp::toFormattedString(bool showMicroseconds) const {
	char buf[64] = { 0 };
	time_t seconds = static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
	seconds += 8 * 60 * 60;
	struct tm tm_time {};
		
	gmtime_r(&seconds, &tm_time);

	
	if (showMicroseconds)
	{
		// fixme: tm_hour lates 8 hours-> seconds += 8 * 60 * 60;
		int microseconds = static_cast<int>(microSecondsSinceEpoch_ % kMicroSecondsPerSecond);
		snprintf(buf, sizeof(buf), "%4d/%02d/%02d %02d:%02d:%02d.%06d",
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
			microseconds);
	}
	else
	{
		snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	}
	return buf;
}

bool Timestamp::valid() const {
	return microSecondsSinceEpoch_ > 0;
}


int64_t Timestamp::microSecondsSinceEpoch() const {
	return microSecondsSinceEpoch_;
}

time_t Timestamp::secondsSinceEpoch() const {
	return static_cast<time_t>(microSecondsSinceEpoch_ / kMicroSecondsPerSecond);
}


// core function : gettimeofday(&timeval, nullptr)
Timestamp Timestamp::now() {
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	int64_t seconds = tv.tv_sec;
	return Timestamp(seconds * kMicroSecondsPerSecond + tv.tv_usec);
}


Timestamp Timestamp::invalid() {
	return Timestamp();
}

Timestamp Timestamp::fromUnixTime(time_t t) {
	return fromUnixTime(t, 0);
}

Timestamp Timestamp::fromUnixTime(time_t t, int microseconds) {
	return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond + microseconds);
}



