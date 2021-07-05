#include "HttpResponse.h"

USE_NAMESPACE

static const char* StatusPhrase(int status);
static std::string Statusline(int status);


std::string HttpResponse::tostring() const {
    std::string respond = Statusline(status_);
    if (closeConnection_)
        respond += "Connection: close\r\n";
    else
        respond += "Connection: Keep-Alive\r\n";

	if(!body_.empty()) {
        respond += "Content-Length: " + std::to_string(body_.size()) + "\r\n";
	}

	for(const auto& it : headers_) {
        const auto line = it.first + ": " + it.second + "\r\n";
        respond += line;
	}

    respond += "\r\n";
    respond += body_;
    return respond;
}

const char* StatusPhrase(int status) {
    switch (status) {
    case 200:
        return "OK";
    case 204:
        return "No Content";
    case 301:
        return "Moved Permanently";
    case 400:
        return "Bad Request";
    case 404:
        return "Not Found";
    default:
        return "Unknown";
    }
}

std::string Statusline(int status) {
	// fixme : only http 1.1
    std::string statusLine = "HTTP/1.1 ";
    statusLine += std::to_string(status);
    statusLine.push_back(' ');
    statusLine += StatusPhrase(status);
    statusLine.append("\r\n");
    return statusLine;
}

