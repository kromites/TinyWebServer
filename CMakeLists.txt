# CMakeList.txt: TinyWebServer 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("TinyWebServer")

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17)

# HTTP的功能模块

set(Request "http/HttpRequest.h" "http/HttpRequest.cpp")
set(Parse "http/HttpParse.h" "http/HttpParse.cpp")
set(Response "http/HttpResponse.h" "http/HttpResponse.cpp")
set(HttpServer "http/HttpServer.h" "http/HttpServer.cpp" ${Request} ${Parse} ${Response})

# TCP的功能模块

set(Address "Address.h" "Address.cpp")
set(Acceptor "Acceptor.h" "Acceptor.cpp")
set(Buffer "Buffer.h" "Buffer.cpp")
set(Channel "Channel.h" "Channel.cpp")
set(Connection "Connection.h" "Connection.cpp")
set(EventLoop "EventLoop.h" "EventLoop.cpp")
set(EventLoopThread "EventLoopThread.h" "EventLoopThread.cpp")
set(EventLoopThreadPool "EventLoopThreadPool.h" "EventLoopThreadPool.cpp")
set(Poller "Poller.h" "Poller.cpp")
set(Socket "Socket.h" "Socket.cpp")
set(TcpServer "TcpServer.h" "TcpServer.cpp")


# 底层封装功能模块 
# feature			-> pthread 
#					-> logger 
set(Util "base/Util.h")
set(Timestamp "base/Timestamp.h" "base/Timestamp.cpp")
set(ThreadSynchronization "base/Mutex.h" "base/Mutex.cpp" "base/Condition.h" "base/Condition.cpp" "base/CountDownLatch.h" "base/CountDownLatch.cpp")
set(Thread "base/CurrentThread.h" "base/CurrentThread.cpp" "base/Thread.h" "base/Thread.cpp")

set(LOG "base/LogStream.h" "base/LogStream.cpp" "base/Logger.h" "base/Logger.cpp"  ${Timestamp})
set(LogFile "base/FileUtil.h" "base/FileUtil.cpp" "base/LogFile.h" "base/LogFile.cpp")
set(AsyncLog "base/AsyncLogging.h" "base/AsyncLogging.cpp" ${LOG} ${LogFile})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread" )


# 底层测试文件
#add_executable(testCondition "base/test/testCondition.cpp" ${Util} ${ThreadSynchronization} ${Thread} ${LOG})
#add_executable(testAtomic "base/test/testAtomic.cpp")
#add_executable(testThread_joinAndDetach "base/test/testThread.cpp") 
#add_executable(testLogger "base/test/testLogger.cpp" ${LOG} ${Thread} ${Util} ${ThreadSynchronization})
#add_executable(testAsyncLogger "base/test/testAsyncLogger.cpp" ${AsyncLog} ${Util} ${Thread} ${ThreadSynchronization})
#add_executable(testFileUtil "base/test/testFileUtil.cpp" "base/FileUtil.h" "base/FileUtil.cpp" ${Util})
#add_executable(testLogFile "base/test/testLogFile.cpp"  ${Util} ${LogFile} ${Thread} ${ThreadSynchronization} ${LOG})
#add_executable(testParse "http/testParse.cpp")


# 将源代码添加到此项目的可执行文件。
#add_executable (test "TinyWebServer.cpp" "TinyWebServer.h" "Poller.h" "base/AsyncLogging.h" "base/AsyncLogging.cpp" "base/LogStream.cpp")
#add_executable(logTest "base/LogStream.h" "base/LogStream.cpp" "test/testLogStream.cpp")
#add_executable(logerTest "base/LogStream.h" "base/LogStream.cpp" "base/Logger.h" "base/Logger.cpp" "test/testLogger.cpp" )
#add_executable(T1 "test/testLogStream.cpp" )
#add_executable(testTid "test/testTid.cpp")
#add_executable(testAny "test/testStdAny.cpp")
#add_executable(testThread  "test/testThread.cpp"  ${LOG} ${ThreadSynchronization} ${Thread})
#add_executable(acceptTest "test/acceptTest.cpp" ${Socket} ${Address} ${LOG})
#add_executable(testAcceptor "test/testAcceptor.cpp" ${Socket} ${Address} ${LOG} ${Acceptor})
#add_executable(testEventLoop "test/testLoop01.cpp" ${Channel} ${Poller} ${EventLoop} ${Thread} ${ThreadSynchronization} ${LOG})
#add_executable(testEventLoop2 "test/testLoop02.cpp" ${Channel} ${Poller} ${EventLoop} ${Thread} ${ThreadSynchronization} ${LOG})
#add_executable(testEventLoop3 "test/testLoop03.cpp" ${Channel} ${Poller} ${EventLoop} ${Thread} ${ThreadSynchronization} ${LOG})
#add_executable(testTimestamp1 "test/testTimestamp.cpp")
#add_executable(testTimestamp2 "test/testTimestamp2.cpp" ${LOG})
#add_executable(testTcpNet "test/testTcpNet.cpp" ${Channel} ${Poller} ${EventLoop} ${Thread} ${ThreadSynchronization} ${LOG} ${Acceptor} ${Address} ${Socket})


#add_executable(main  
#					${Address} ${Acceptor} ${Buffer} ${Channel} ${Connection} 
#					${EventLoop} ${EventLoopThread} ${EventLoopThreadPool} 
#					${Poller}  ${Socket} ${TcpServer}
#					${LOG} ${Thread} ${ThreadSynchronization} ${Timestamp} ${Util})


#add_executable(Echo "test/TestEcho.cpp" 
#					${Address} ${Acceptor} ${Buffer} ${Channel} ${Connection} 
#					${EventLoop} ${EventLoopThread} ${EventLoopThreadPool} 
#					${Poller}  ${Socket} ${TcpServer}
#					${AsyncLog} ${Thread} ${ThreadSynchronization} ${Timestamp} ${Util})


add_executable(Http "http/TestHttpServer.cpp" 
					${HttpServer}
					${Address} ${Acceptor} ${Buffer} ${Channel} ${Connection} 
					${EventLoop} ${EventLoopThread} ${EventLoopThreadPool} 
					${Poller} ${Socket} ${TcpServer}
					${AsyncLog} ${Thread} ${ThreadSynchronization} ${Timestamp} ${Util})

# TODO: 如有需要，请添加测试并安装目标。
