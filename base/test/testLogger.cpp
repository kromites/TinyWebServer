#include <unistd.h>

#include "../Logger.h"
#include "../Timestamp.h"

USE_NAMESPACE

int main() {
	int n = 1000;
	Timestamp timestamp(Timestamp::now());
	
	for(int i=0; i<n; ++i) {
		LOG_DEBUG << "print i = " << i;
	}
	sleep(60);
	return 0;
}