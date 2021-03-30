# CMakeList.txt: TinyWebServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("TinyWebServer")

set(LOG "base/LogStream.h" "base/LogStream.cpp" "base/Logger.h" "base/Logger.cpp" )
set(Poller "Poller.h" "Poller.cpp")
set(Address "Address.h" "Address.cpp")
set(Socket "Socket.h" "Socket.cpp")
set(Buffer "Buffer.h" "Buffer.cpp")


# 将源代码添加到此项目的可执行文件。
add_executable (test "TinyWebServer.cpp" "TinyWebServer.h" "Poller.h" "base/AsyncLogging.h" "base/AsyncLogging.cpp" "base/LogStream.cpp")
add_executable(logTest "base/LogStream.h" "base/LogStream.cpp" "test/testLogStream.cpp")
add_executable(logerTest "base/LogStream.h" "base/LogStream.cpp" "base/Logger.h" "base/Logger.cpp" "test/testLogger.cpp" )
add_executable(T1 "test/testLogStream.cpp" )
add_executable(main "TestServer.cpp" ${LOG} ${Address} ${Socket} ${Poller} ${Buffer} )
add_executable(testTid "test/testTid.cpp")

# TODO: 如有需要，请添加测试并安装目标。
