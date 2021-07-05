#include <thread>
#include <iostream>
#include <unistd.h>


void myPrintf() {
    sleep(1);
    std::cout << "threadRun" << std::endl;
    return;
}

int main()
{
    std::thread t(myPrintf);
    t.join();   //15
    std::cout << "main" << std::endl;
    sleep(100);
    return 0;
}
/*�����
threadRun
main
�밴���������. . .
*/