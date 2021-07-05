#pragma once
#include "Util.h"

#include <cstdint>
#include <string>

START_NAMESPACE

constexpr int kMicroSecondsPerSecond = 1000 * 1000;

class Timestamp {
public:
	Timestamp();
	explicit Timestamp(int64_t microSecondsSinceEpochArg);

	// is useful?
	void swap(Timestamp& that);

	std::string toString() const;
	std::string toFormattedString(bool showMicroseconds = true) const;

	bool valid() const;

	// for internal usage.
	int64_t microSecondsSinceEpoch() const;
	time_t secondsSinceEpoch() const;


	// Get time of now!
	static Timestamp now();
	static Timestamp invalid();

	static Timestamp fromUnixTime(time_t t);
	static Timestamp fromUnixTime(time_t t, int microseconds);

private:
	int64_t microSecondsSinceEpoch_;
};


// operator

inline bool operator<(Timestamp& lhs, Timestamp& rhs) {
	return lhs.microSecondsSinceEpoch() < rhs.microSecondsSinceEpoch();
}

inline bool operator==(Timestamp& lhs, Timestamp& rhs) {
	return lhs.microSecondsSinceEpoch() == rhs.microSecondsSinceEpoch();
}

inline bool operator>(Timestamp& lhs, Timestamp& rhs) {
	return lhs.microSecondsSinceEpoch() > rhs.microSecondsSinceEpoch();
}

inline double timeDifference(Timestamp& high, Timestamp& low) {
	auto diff = high.microSecondsSinceEpoch() - low.microSecondsSinceEpoch();
	return static_cast<double>(diff) / kMicroSecondsPerSecond;
}

inline Timestamp addTime(Timestamp& timestamp, double seconds) {
	auto currentMicroseconds = static_cast<int64_t>(seconds * kMicroSecondsPerSecond);
	return Timestamp(timestamp.secondsSinceEpoch()+currentMicroseconds);
}

END_NAMESPACE