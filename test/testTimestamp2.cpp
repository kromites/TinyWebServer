#include "../base/Timestamp.h"
#include "../base/Logger.h"

#include <unistd.h>

int main() {
	Timestamp time(Timestamp::now());

	LOG_TRACE << time.toFormattedString();
	sleep(60);
}