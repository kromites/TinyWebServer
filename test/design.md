

// 将 阻塞队列也封装为文件描述符
BQ blockqueue	// ITC

class BQ{
	int fd;

	void add(int conn){
		// write one byte to fd
	}

	int consume(){
		// read all bytes from fd
	}
}


// consumer
// thread func for connection
void ioThreadFunc(){

begin:
	
	// manage connection, (but how?)
		epoll_wait	// block!
		handle active connection
			situation 1: get message from client, call message callback
			situation 2: client close connection, call close callback,
			             erase connection from epoll, then close it
	goto begin 
}


// producer
// thead func for get new conneciton
void acceptThreadfunc(){

start:
	
	// get new connection
	// add connection to epoll
	goto start
}



thread t1(ioThreadFunc)
thread t2(acceptThreadfunc)