# CMakeList.txt: TinyWebServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("TinyWebServer")

set(LOG "base/LogStream.h" "base/LogStream.cpp" "base/Logger.h" "base/Logger.cpp" "base/CurrentThread.h" "base/CurrentThread.cpp")

set(Address "Address.h" "Address.cpp")
set(Acceptor "Acceptor.h" "Acceptor.cpp")
set(Buffer "Buffer.h" "Buffer.cpp")
set(Channel "Channel.h" "Channel.cpp")
set(EventLoop "EventLoop.h" "EventLoop.cpp")
set(Poller "Poller.h" "Poller.cpp")
set(Socket "Socket.h" "Socket.cpp")



set(ThreadSynchronization "base/Mutex.h" "base/Mutex.cpp" "base/Condition.h" "base/Condition.cpp" "base/CountDownLatch.h" "base/CountDownLatch.cpp")
set(Thread "base/CurrentThread.h" "base/CurrentThread.cpp" "base/Thread.h" "base/Thread.cpp")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11  -pthread" )

# 将源代码添加到此项目的可执行文件。
add_executable (test "TinyWebServer.cpp" "TinyWebServer.h" "Poller.h" "base/AsyncLogging.h" "base/AsyncLogging.cpp" "base/LogStream.cpp")
add_executable(logTest "base/LogStream.h" "base/LogStream.cpp" "test/testLogStream.cpp")
add_executable(logerTest "base/LogStream.h" "base/LogStream.cpp" "base/Logger.h" "base/Logger.cpp" "test/testLogger.cpp" )
add_executable(T1 "test/testLogStream.cpp" )
add_executable(testTid "test/testTid.cpp")
add_executable(testThread  "test/testThread.cpp"  ${LOG} ${ThreadSynchronization} ${Thread})
add_executable(acceptTest "test/acceptTest.cpp" ${Socket} ${Address} ${LOG})
add_executable(testAcceptor "test/testAcceptor.cpp" ${Socket} ${Address} ${LOG} ${Acceptor})
add_executable(testEventLoop "test/testLoop01.cpp" ${Channel} ${Poller} ${EventLoop} ${Thread} ${ThreadSynchronization} ${LOG})
add_executable(testEventLoop2 "test/testLoop02.cpp" ${Channel} ${Poller} ${EventLoop} ${Thread} ${ThreadSynchronization} ${LOG})
add_executable(testEventLoop3 "test/testLoop03.cpp" ${Channel} ${Poller} ${EventLoop} ${Thread} ${ThreadSynchronization} ${LOG})

add_executable(main "TestServer.cpp" ${LOG} ${Address} ${Socket} ${Channel} ${Poller} ${EventLoop} ${Buffer} ${Acceptor} ${Thread} ${ThreadSynchronization})
# TODO: 如有需要，请添加测试并安装目标。
