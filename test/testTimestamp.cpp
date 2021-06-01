#include<iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

// int main() {
//     struct timeval tv;
//     gettimeofday(&tv, NULL);
//     printf("second:%ld\n", tv.tv_sec);  //秒
//     printf("millisecond:%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);  //毫秒
//     printf("microsecond:%ld\n", tv.tv_sec * 1000000 + tv.tv_usec);  //微秒
//
//     sleep(3); // 为方便观看，让程序睡三秒后对比
//     std::cout << "3s later:" << std::endl;
//
//     gettimeofday(&tv, NULL);
//     printf("second:%ld\n", tv.tv_sec);  //秒
//     printf("millisecond:%ld\n", tv.tv_sec * 1000 + tv.tv_usec / 1000);  //毫秒
//     printf("microsecond:%ld\n", tv.tv_sec * 1000000 + tv.tv_usec);  //微秒
//
//     sleep(60);
//     return 0;
// }

#include <stdio.h>
#include <time.h>
int main(void)
{
	char buf[64] = { 0 };
	struct tm tm_time;
	time_t lt;
	lt = time(NULL);
	tm_time = *localtime(&lt);
	snprintf(buf, sizeof(buf), "%4d/%02d/%02d %02d:%02d:%02d",
			tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
			tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	std::cout << buf << std::endl;
	return 0;
}