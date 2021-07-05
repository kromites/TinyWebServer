#pragma once
/*
 *	The httpResponse class if for analyze http response message
 *	The response message include state line, response header and response body
 *
 *	for example
 * ******************************************************************************
 *																				*
 *	state line :		HTTP Version + status code + status phrase				*	
 *																				*
 *	response header:	Server : Apache											*			
 *						...														*
 *	space line:			CR + LF													*
 *																				*
 *	response body:		<html> ... </html>										*
 *																				*
 * ******************************************************************************
 */

#include <map>
#include <string>

#include "../base/Util.h"

START_NAMESPACE

class Buffer;

class HttpResponse {
public:
	using Headers = std::map<std::string, std::string>;

	HttpResponse(): status_(400), closeConnection_(false) {}

	int status() { return status_; }
	// std::string phrase() { return phrase_; }
	// header: no judge and no get, only put in
	void addHeader(const std::string& header, const std::string& value) {
		headers_[header] = value;
	}
	std::string& body() { return body_; }
	bool& closeConnection() { return closeConnection_; }

	// tostring
	std::string tostring() const;

	// set variable
	void setStatus(int status) { status_ = status; }
	void setCloseConnection(bool closeConnection) { closeConnection_ = closeConnection; }
	void setBody(const std::string& body) { body_ = body; }

private:
	int status_;
	// std::string phrase_;
	Headers headers_;
	std::string body_;

	bool closeConnection_;
};


END_NAMESPACE

