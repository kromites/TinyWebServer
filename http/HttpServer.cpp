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
		if (conn->context_.type() == typeid(HttpParse)) {
			LOG_DEBUG << "context's type is HttpParse ";
		}else {
			LOG_DEBUG << "context's type is not HttpParse";
		}
	}
}

void HttpServer::onRequest(const ConnectionPtr& conn, HttpRequest& request, const std::string& message) {
	// const auto& connection = request.getHeader("Connection");
	// const auto close = (connection == "close") || (request.version() == HttpRequest::kHttp10 && (connection != "Keep-Alive" || connection != "keep-alive"));
	HttpResponse response;
	response.setStatus(200);
	response.setBody(message);
	response.setCloseConnection(true);
	conn->send(response.tostring());
	if(response.closeConnection()) {
		conn->shutdown();
	}
}

void HttpServer::onMessage(const ConnectionPtr conn, Buffer& buf, const std::string& message) {
	
	HttpParse* parser = std::any_cast<HttpParse>(conn->getMutableContext());
	parser->initState();
	// LOG_DEBUG << "parse init state" << parser->state();
	if(!parser->parseRequest(buf)) {
		conn->send("HTTP/1.1 400 Bad Request\r\n\r\n");
		conn->shutdown();
	}

	if(parser->getAll()) {
		auto& req = parser->request();
		onRequest(conn, req, message);
		parser->reset();
	}
}





