#include "HttpParse.h"
#include "../Buffer.h"
#include "../base/Logger.h"


#include <algorithm>
USE_NAMESPACE

static const char* findCRLF(const char* begin, const char* end) {
	const auto len = end - begin;
	for (int i = 0; i < len - 1; ++i) {
		if (begin[i] == '\r' && begin[i + 1] == '\n') {
			return begin + i;
		}
	}
	return nullptr;
}

bool HttpParse::parseRequest(Buffer& buffer) {
	LOG_TRACE << "start parse request";
	auto ret = parse(buffer.readString(), buffer.readString() + buffer.ReadableSize());
	// handle the ret
	LOG_TRACE << "This parse consume " << (ret > 0 ? ret : -1) << " bytes";
	if (ret < 0) {
		return false;
	}
	buffer.consume(ret);
	return true;
}

int HttpParse::parse(const char* begin, const char* end) {
	LOG_TRACE << "This is parse circle function ";
	bool hasMore = true;
	bool ok = true;
	const char* start = begin;
	int cnt = 0;
	while (hasMore) {
		if (state_ == kREQUESTLINE) {
			const char* crlf = findCRLF(start, end);
			if (crlf) {
				ok = parseRequestLine(start, crlf);
				if (ok) {
					state_ = kHEADER;
					start = crlf + 2;
				}
				else {
					hasMore = false;
				}
			}
			else {
				hasMore = false;
			}
		}
		else if (state_ == kHEADER) {
			int ret = parseHeader(start, end);
			if (ret > 0) {
				start += ret;
			}
			else if (ret == 0) {
				// empty line, end of header
				// fixme
				state_ = kALL;
				start += 2;
				hasMore = false;
			}
			else {
				// ret == -1 parse header fails 
				ok = false;
				break;
			}
		}
		else if (state_ == kBODY) {
			if (request_.method() == HttpRequest::kGet || request_.method() == HttpRequest::kHead) {
				// no body in GET method, GotAll
				state_ = kALL;
			}
			// else if (request_.method() != HttpRequest::kInvalid) {
			// 	// get body from message
			// 	auto it = request_.header().find("Content-Length");
			// 	if (it == request_.header().end())
			// 		return -1;
			// 	remainLength_ = std::stoi(it->second);
			// 	auto len = end - start;
			// 	if (len < remainLength_) {
			// 		// not full message
			// 		request_.body() = std::string(start, end);
			// 		start = end;
			// 		remainLength_ -= len;
			// 	}
			// 	else {
			// 		request_.body() = std::string(start, remainLength_);
			// 		start += remainLength_;
			// 		remainLength_ = 0;
			// 		state_ = ALL;
			// 	}

			// }
		}
		cnt++;
	}

	return start - begin;
}


bool HttpParse::parseRequestLine(const char* begin, const char* end) {
	LOG_TRACE << "start parse request line";
	bool succeed = false;
	const char* start = begin;
	const char* space = std::find(start, end, ' ');
	if (space != end && request_.setMethod(start, space)) {
		LOG_TRACE << request_.toString(request_.method());
		
		start = space + 1;
		space = std::find(start, end, ' ');
		// find the path
		if (space != end) {
			request_.setUrl(start, space);
			LOG_TRACE << request_.URL();
			start = space + 1;
			succeed = (end - start == 8) && std::equal(start, end - 1, "HTTP/1.");
			if (succeed) {
				if (*(end - 1) == '0') {
					request_.setVersion(HttpRequest::kHttp10);
				}
				else if (*(end - 1) == '1') {
					request_.setVersion(HttpRequest::kHttp11);
				}
				else {
					succeed = false;
				}
			}
			LOG_TRACE << request_.toString(request_.version());
		}
	}
	return succeed;
}

int HttpParse::parseHeader(const char* begin, const char* end) {
	LOG_TRACE << "start parse header";
	const char* crlf = findCRLF(begin, end);
	if (crlf == nullptr) {
		// crlf can't find.
		return -1;
	}
	if (crlf - begin > 0) {
		const char* colon = std::find(begin, crlf, ':');
		if (colon != crlf) {
			const auto* left = colon - 1;
			while (*left == ' ') --left;

			const auto* right = colon + 1;
			while (*right == ' ') ++right;
			std::string header(begin, left + 1), value(right, crlf);
			request_.addHeader(header, value);
		}
	}
	else {
		// end of header
		return 0;
	}

	auto ret = crlf - begin;
	return ret > 0 ? ret + 2 : 0;
}
