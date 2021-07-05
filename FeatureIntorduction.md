## 本项目功能介绍



### 1. 前言

​	本项目是阅读完成《APUE》和《UNP》第一卷，对网络编程有一定的了解之后，仿照陈硕先生写的muduo网络库所写出来的小型高性能Web服务器。

​	该项目是从下往上写的，先对一些基本概念进行封装，写好TCP层对连接的处理，再去写HTTP的请求和响应部分。

```
三步走
-> base
-> Tcp
-> Http
```



### 2. 基础库封装介绍

- 线程类及线程同步封装：

  简单封装Pthread库中的的线程`thread`和几个线程同步方法：`mutex`，`condition`和`CountDownLatch`

- 异步日志库的完成

  - 实现多级别日志消息（`Logger`)
  - 实现基于前后端双缓冲区的异步日志库（`AsyncLogging`)



### 3.  网络库设计部分



### 4. HTTP报文处理部分











## 需要改进地方

截止到七月初，目前需要改进的地方有

- FATAL: HTTP服务器消息回调函数中将一次连接中的HttpParse和Connection绑定时使用std::any出现语法错误或者段错误。

- BUG: 修改异步日志在项目中不能写入日志文件的bug

  附：启动异步日志时钟在标准输出日志消息，但是做单独的异步日志功能测试却能写入本地文件。

- WARN : 添加定时器

  目前关于时间部分基本没做，只是做了一个Timestamp来在日志库中表示当前时间

- INFO：HTTP层功能很单一，现在只是简单支持GET来尝试能不能跑通，之后希望能继续改进成能够上传文件的服务器。
