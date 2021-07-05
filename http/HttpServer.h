// todo: complete a http sever

/*
 * The HttpServer class Encapsulates tcpSever
 * 
 * HttpServer feature: 
 *					-> accept http request message from client						-> onConnection
 *					-> parse http request message									-> onRequest
 *					-> locate resource and put the resource in response message		-> onRequest
 *					-> send the http response message to client						-> onMessage
 *
 */

#include "../base/Util.h"
#include "../TcpServer.h"

START_NAMESPACE

class HttpRequest;
class HttpResponse;

class HttpServer {
private:
	using ConnectionPtr = std::shared_ptr<Connection>;
	using ConnectionCallback = std::function<void(ConnectionPtr)>;
	using MessageCallback = std::function<void(ConnectionPtr, Buffer&)>;

public:
	HttpServer(EventLoop* loop, int port, int workthread);
	// set callback function
	void setConnectionCallback(const ConnectionCallback& cb) { server_.setConnectionCallback(cb); }
	void setMessageCallback(const MessageCallback& cb) { server_.setMessageCallback(cb); }

	void start();

	// init callback function
	void onConnection(const ConnectionPtr conn);
	void onRequest(const ConnectionPtr& conn, HttpRequest&, const std::string& message);
	void onMessage(const ConnectionPtr conn, Buffer& buf, const std::string& message);
private:
	TcpServer server_;
};

END_NAMESPACE