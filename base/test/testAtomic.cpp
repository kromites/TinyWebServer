#include <thread>
#include <atomic>
#include <iostream>
#include <list>
#include <unistd.h>

using namespace std;

atomic_int iCount(0);

void threadfun1()
{
    for (int i = 0; i < 1000; i++)
    {
        ("iCount:%d\r\n", iCount++);
    }
}
void threadfun2()
{
    for (int i = 0; i < 1000; i++)
    {
        printf("iCount:%d\r\n", iCount--);
    }
}

int main()
{
    std::list<thread> lstThread;
    for (int i = 0; i < 100; i++)
    {
        lstThread.push_back(thread(threadfun1));
    }
    for (int i = 0; i < 100; i++)
    {
        lstThread.push_back(thread(threadfun2));
    }

    for (auto& th : lstThread)
    {
        th.join();
    }

    //printf("finally iCount:%d\r\n",  iCount);
    int x = iCount.load(memory_order_relaxed);
    printf("finally iCount:%d\r\n", x);


    sleep(60);
    return 0;
}