#include "../Util.h"

#include "../Mutex.h"
#include "../Condition.h"

#include "../Thread.h"
#include "../CurrentThread.h"

#include "../Logger.h"

#include <queue>
#include <unistd.h>
USE_NAMESPACE

MutexLock mutex;
Condition cond(mutex);
std::queue<int> queue;

int dequeue() {
	MutexLockGuard guard(mutex);
	while(queue.empty()) {
		
		LOG_DEBUG << "start wait";
		cond.wait();

		LOG_DEBUG << "wait is over";
	}
	int front = queue.front();

	sleep(1);
	queue.pop();



	LOG_DEBUG << "thread " << CurrentThread::tid() << " has pop one element : " << front;
	return front;
}

void enqueue(int x) {
	MutexLockGuard guard(mutex);
	queue.push(x);
	cond.notifyAll();
}

int main() {

	
	Thread t1(dequeue);
	Thread t2(dequeue);

	t1.start();
	t2.start();
	
	enqueue(1);
	LOG_DEBUG << "thread " << CurrentThread::tid() << " has push one element : 1";

	
	t1.join();
	t2.join();
	LOG_DEBUG << "end";

	sleep(60);
	return 0;

}