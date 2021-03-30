#include <unistd.h>

#include <iostream>
#include <pthread.h>
#include <sys/syscall.h>

using namespace std;
int main() {


    cout << pthread_self() << endl;

    cout << ::gettid() << endl;

    cout << syscall(SYS_gettid);
}