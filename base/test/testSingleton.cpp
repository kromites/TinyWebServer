#include "../Util.h"
#include "../Logger.h"

START_NAMESPACE

template<typename T>
class Singleton {
public:
	Singleton() = delete;
	~Singleton() = delete;
	
	static T& instance() {
		if(value_ == nullptr) {
			value_ = new T();
		}
		return value_;
	}

	

private:
	static pthread_once_t ponce_;
	static T* value_;

	
};




END_NAMESPACE