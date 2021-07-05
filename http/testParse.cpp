#include <iostream>
#include <memory>
#include <any>
#include <complex>
#include <functional>

using std::cout;
using std::endl;

class HttpParse {
public:
	enum ParseState {
		REQUESTLINE = 0,
		HEADER,
		BODY,
		ALL
	};
	
	HttpParse() :state_(REQUESTLINE) {}
	void test() {
		if(state_ == REQUESTLINE) {
			std::cout << "no problem!" << std::endl;
		}
	}
private:
	ParseState state_;
};


class conn {
public:
	template <typename T>
	void setContext(T&& value) {
		context_ = std::make_any<T>(std::forward<T>(value));
	}

	std::any& getContext() {
		return context_;
	}
	std::any context_;
};

int main() {
	std::shared_ptr<conn> co = std::make_shared<conn>();
	
	co->setContext(HttpParse());
	cout << (co->context_.type() == typeid(HttpParse)) << endl;
	HttpParse& parser = std::any_cast<HttpParse&>(co->getContext());
	parser.test();

	std::string a = "Hello, std::any!\n";
	auto a0 = std::make_any<std::string>(a);
	auto a1 = std::make_any<std::complex<double>>(0.1, 2.3);

	std::cout << std::any_cast<std::string&>(a0);
	std::cout << std::any_cast<std::complex<double>&>(a1) << '\n';

	using lambda = std::function<void(void)>;

	// Put a lambda into std::any. Attempt #1 (failed).
	std::any a2 = [] { std::cout << "Lambda #1.\n"; };
	std::cout << "a2.type() = \"" << a2.type().name() << "\"\n";

	// any_cast casts to <void(void)> but actual type is not
	// a std::function..., but ~ main::{lambda()#1}, and it is
	// unique for each lambda. So, this throws...
	try {
		std::any_cast<lambda>(a2)();
	}
	catch (std::bad_any_cast const& ex) {
		std::cout << ex.what() << '\n';
	}

	// Put a lambda into std::any. Attempt #2 (successful).
	auto a3 = std::make_any<lambda>([] { std::cout << "Lambda #2.\n"; });
	std::cout << "a3.type() = \"" << a3.type().name() << "\"\n";
	std::any_cast<lambda>(a3)();
	
	return 0;
}

