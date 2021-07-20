#include "../AsyncLogging.h"
#include "../Logger.h"

#include <sys/resource.h>
#include <unistd.h>

#include "../LogFile.h"

USE_NAMESPACE

AsyncLogging* g_asyncLog = nullptr;

void asyncOutput(const char* msg, size_t len)
{
    g_asyncLog->write(msg, len);
}


// void bench(bool longLog)
// {
//     Logger::setOutput(asyncOutput);
//
//     int cnt = 0;
//     const int kBatch = 1000;
//     std::string empty = " ";
//     std::string longStr(3000, 'X');
//     longStr += " ";
//
//     for (int t = 0; t < 30; ++t)
//     {
//         for (int i = 0; i < kBatch; ++i)
//         {
//             LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
//                 << (longLog ? longStr : empty)
//                 << cnt;
//             ++cnt;
//         }
//     }
// }

int main()
{
    // {
    //     // set max virtual memory to 2GB.
    //     size_t kOneGB = 1000 * 1024 * 1024;
    //     rlimit rl = { 2 * kOneGB, 2 * kOneGB };
    //     setrlimit(RLIMIT_AS, &rl);
    // }

    std::string basename = "../../../Log/testAsync3.log";
	AsyncLogging log(basename);
    log.start();
    g_asyncLog = &log;

    Logger::setOutput(asyncOutput);
    
	

    int cnt = 0;
    const int kBatch = 10;
    std::string empty = " ";

    for (int t = 0; t < 300; ++t)
    {
        for (int i = 0; i < kBatch; ++i)
        {
            LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz "
                << cnt;
            ++cnt;
        }
    }


    return 0;
}
