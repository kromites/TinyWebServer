#pragma once
/*
 *	The HttpParse class is for parse the http request message
 *	first we use some enums to record.
 *		ParseState: record what we are parsing currently.
 *		HttpCode: record the success or failure of parse.
 *		LineStatus: record the situation of parse line.
 *	second we use state machine method to parse the http request message 
 * 
 */

// todo fix parse bug: review the parse 

#include "../base/Util.h"
#include "HttpRequest.h"

START_NAMESPACE

class Buffer;

class HttpParse {
public:
	enum ParseState {
		kREQUESTLINE = 0,
		kHEADER,
		kBODY,
		kALL,
	};
	
	HttpParse(): state_(kREQUESTLINE) {}

	bool getAll() const {
		return state_ == kALL;
	}

	void reset() {
		state_ = kREQUESTLINE;
		request_.clear();
	}

	HttpRequest& request() { return request_; }
	const HttpRequest& request() const { return request_; }
	bool parseRequest(Buffer& buffer);

	std::string state() {
		std::string ret;

		if (state_ == kREQUESTLINE)
			ret = "Request Line";
		else if (state_ == kHEADER)
			ret = "Header";
		else if (state_ == kBODY)
			ret = "Body";
		else
			ret = "All";
		return ret;
	}

	void initState() {
		state_ = kREQUESTLINE;
	}

private:
	int parse(const char* begin, const char* end);
	bool parseRequestLine(const char* begin, const char* end);
	int parseHeader(const char* begin, const char* end);

	ParseState state_;
	HttpRequest request_;
	// int remainLength_ = -1;
};

END_NAMESPACE
