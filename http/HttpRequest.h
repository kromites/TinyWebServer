#pragma once
/*
 *	The HttpRequest class is for analyze http request message.
 *	http request message include request line, request header and request body.
 *
 *	for example
 ************************************************************************
 *	request line : METHOD + URI + HTTP version							*
 *	request header:														*			
 *						Host : localhost								*
 *						...												*
 *						CR + LF											*
 *	request content:	(not necessary)									*
 *																		*
 *																		*	
 ************************************************************************
 *
 *	from the example, we can use enum to save method and version, and use the map to save request header.
 *					
 */


#include "../base/Util.h"

#include <map>
#include <string>

START_NAMESPACE

class HttpRequest {
public:
	enum Method {
		kGet = 0,
		kPost,
		kPut,
		kHead,
		kDelete,
		kInvalid
	};

	enum Version {
		kUnknown = 0,
		kHttp10,
		kHttp11
	};

	using Headers = std::map<std::string, std::string>;
	
	HttpRequest(): method_(kInvalid), version_(kUnknown) { };
	~HttpRequest() = default;


	Method method() const { return method_; }
	Version version() const { return version_; }
	std::string URL() const { return uri_; }
	std::string body() const { return body_; }
	Headers header() const { return headers_; }

	bool setMethod(const char* start, const char* end);
	void setVersion(Version version) { version_ = version; }
	void setUrl(const char* start, const char* end) { uri_.assign(start, end); }
	void setBody(const char* start, const char* end) { body_.assign(start, end); }

	bool isHeader(std::string& header) const;
	void addHeader(const std::string& header, const std::string& value);
	std::string getHeader(std::string& header) const;
	std::string getHeader(std::string&& header) const { return getHeader(header); }

	void swap(HttpRequest& request) noexcept;
	friend void swap(HttpRequest& lhs, HttpRequest& rhs) noexcept;
	void clear();
	
	inline std::string toString(Method method) {
		switch (method) {
		case Method::kInvalid:
			return "Invalid Method";
			break;
		case Method::kGet:
			return "GET";
			break;
		case Method::kPost:
			return "POST";
			break;
		case Method::kHead:
			return "HEAD";
			break;
		case Method::kDelete:
			return "DELETE";
			break;
		case Method::kPut:
			return "PUT";
			break;
		}
	}

	inline std::string toString(Version version) {
		switch (version) {
		case Version::kUnknown:
			return "Unknown Version";
			break;
		case Version::kHttp10:
			return "HTTP/1.0";
			break;
		case Version::kHttp11:
			return "HTTP/1.1";
			break;
		}
	}
	

private:
	Method method_;
	Version version_;
	std::string uri_;
	Headers headers_;
	std::string body_;
};


END_NAMESPACE


