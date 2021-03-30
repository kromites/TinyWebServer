#include "CurrentThread.h"


#include <unistd.h>
#include <sys/syscall.h>

int CurrentThread::tid() {
	return syscall(SYS_gettid);
}
