#include "HttpServer.h"

#include "HttpParse.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "../Connection.h"

USE_NAMESPACE


HttpServer::HttpServer(EventLoop* loop, int port, int workthread) :
	server_(loop, port, workthread)
{
}

void HttpServer::start() {
	LOG_WARN << "HttpServer[" << server_.name() << "] ";
	server_.start();
}

void HttpServer::onConnection(const ConnectionPtr conn) {
	if(conn->connected()) {
		LOG_DEBUG << "HTTP OnConnection ";
		conn->setContext(HttpParse());
	}
}

void HttpServer::onRequest(const ConnectionPtr& conn, HttpRequest& request, const std::string& message) {
	const auto connection = request.getHeader("Connection");
	LOG_WARN << "connection : " << connection;
	const auto close = (connection == "close") || (request.version() == HttpRequest::kHttp10 && (connection != "Keep-Alive" || connection != "keep-alive"));
	LOG_WARN << "close = " << (close ? "true " : "false");
	HttpResponse response;
	response.setStatus(200);
	response.setBody(message);
	response.setCloseConnection(close);
	conn->send(response.tostring());
	if(response.closeConnection()) {
		conn->shutdown();
	}
}

void HttpServer::onMessage(const ConnectionPtr conn, Buffer& buf, const std::string& message) {
	LOG_TRACE << "start On Message";
	auto parser = std::any_cast<HttpParse>(conn->getMutableContext());
	// LOG_DEBUG << "parse init state" << parser->state();
	auto ret = parser->parseRequest(buf);
	LOG_TRACE << "parseRequest " << ( ret ? "success" : "fail");
	if(!ret) {
		LOG_TRACE << "parse fail, return 400 code status";
		conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
		conn->shutdown();
	}

	LOG_TRACE << "state : " << parser->state();
	
	if(parser->getAll()) {
		LOG_TRACE << " parse completed ";
		auto& req = parser->request();
		onRequest(conn, req, message);
		parser->reset();
	}
}






