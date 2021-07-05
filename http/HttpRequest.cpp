#include "HttpRequest.h"
#include <assert.h>


USE_NAMESPACE


bool HttpRequest::setMethod(const char* start, const char* end) {
    assert(method_ == kInvalid);
    std::string s(start, end);
    if (s == "GET")
    {
        method_ = kGet;
    }
    else if (s == "POST")
    {
        method_ = kPost;
    }
    else if (s == "HEAD")
    {
        method_ = kHead;
    }
    else if (s == "PUT")
    {
        method_ = kPut;
    }
    else if (s == "DELETE")
    {
        method_ = kDelete;
    }
    else
    {
        method_ = kInvalid;
    }
    return method_ != kInvalid;
}

void HttpRequest::addHeader(const std::string& header, const std::string& value) {
	headers_[header] = value;
}

std::string HttpRequest::getHeader(std::string& header) const {
    const auto it = headers_.find(header);
    if (it == headers_.end())
        return "";
    return it->second;
}

bool HttpRequest::isHeader(std::string& header) const {
    return headers_.find(header) != headers_.end();
}

void HttpRequest::swap(HttpRequest& request) noexcept {
    std::swap(method_, request.method_);
    std::swap(version_, request.version_);
    headers_.swap(request.headers_);
    uri_.swap(request.uri_);
    body_.swap(request.body_);
}

void HttpRequest::clear() {
    HttpRequest temp;
    this->swap(temp);
}

void swap(HttpRequest& lhs, HttpRequest& rhs) noexcept {
    lhs.swap(rhs);
}



