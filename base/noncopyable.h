#pragma once

class noncopyable {
protected:
	noncopyable();
	~noncopyable();
public:
	noncopyable(const noncopyable&) = delete;
	const noncopyable& operator=(const noncopyable&) = delete;
};