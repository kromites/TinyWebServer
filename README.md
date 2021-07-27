# TinyWebServer

## Introduction

该项目是通过学习陈硕先生写的《muduo》网络库，使用C++编写的简易版多线程服务器。


## Build
- `git clone xxx`
- `./build.sh`
在该目录的父级目录下会生成一个`build/Debug`文件夹，进入该文件夹后会有一系列可执行文件

## Usage

- Echo服务器(单线程) :

  ```shell
  ./ECHO
  ```

- 简易HTTP服务器（单线程，端口已指定）: 

  ```
  ./Http
  ```

- 自定义HTTP服务器：

  `./Main [-t thread_numbers] [-p port] [-l log_file_path(should begin with '/')]`
  (该服务器下的日志文件放在Log目录之下)

## WebBench

在`webbenchResult`文件夹中会记录一些压测结果(todo)

## More

- 增加一些HTTP层面的功能（todo）
- 实现定时器全覆盖（todo）