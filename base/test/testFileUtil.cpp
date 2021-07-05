#include "iostream"
#include "../FileUtil.h"
#include <unistd.h>

USE_NAMESPACE

int main() {
	char buf[2048];
	auto cwd = getcwd(buf, sizeof buf);

	std::cout << cwd << std::endl;
	
	MyFile file("/home/kromite/WebServerLog/test.txt");
	std::string mainPage = "hello world asdas d";

	file.write(mainPage.c_str(), mainPage.size());
	std::cout << file.writeBytes() << std::endl;
	file.flush();
	return 0;
}