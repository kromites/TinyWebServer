#include <vector>


constexpr int InitialSize = 1024;

class Buffer {
public:
	Buffer();

	~Buffer() = default;

	bool empty();
	void reset();

	void writeSize(int);
	void readSize(int);
	
	char* writeStart();
	char* readString();

	size_t size();
	size_t ReadableSize();
	size_t WritableSize();

	
	
private:
	void checkCapacity(int len);


	std::vector<char> buffer_;
	size_t readIndex_;
	size_t writeIndex_;
};