#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 4096

enum CHECK_STATE {
	CHE_STATE_REQUESTLINE = 0,
	CHECK_STATE_HEADER
};

enum LINE_STATUE {
	LINE_OK = 0,
	LINE_BAD,
	LINE_OPEN
};

enum HTTP_CODE {
	NO_REQUEST,
	GET_REQUEST,
	BAD_REQUEST,
	FORBIDDEN_REQUEST,
	INTERNAL_ERROR,
	CLOSED_CONNECTION
};

static const char* szret[] = { "I get a correct result\n", "Something wrong\n" };

// 从状态机，用于解析一行内容，返回解析的行状态。
LINE_STATUE parse_line(char* buffer, int& checked_index, int& read_index){
	char temp;
	/* check_index 指向buffer正在读的字节， read_index指向客户数据尾部的下一个字节， 0-check_index都已经被解析完。 从 check_index - read_index 字节由下面循环分析*/

	for( ; checked_index < read_index; ++checked_index) {
		temp = buffer[checked_index];
		// 回车行，那么之前读到的可能是完整行
		if(temp == '\r') {
			// 到这里就结束，不是完整行
			if((checked_index + 1) == read_index) {
				return LINE_OPEN;
			}else if(buffer[checked_index + 1] == '\n') {
				buffer[checked_index++] = '\0';
				buffer[checked_index++] = '\0';
				return LINE_OK;
			}else {
				// 语法问题
				return LINE_BAD;
			}
		}else if(temp == '\n') {
			if((checked_index > 1) && buffer[checked_index-1] == '\r') {
				buffer[checked_index - 1] = '\0';
				buffer[checked_index++] = '\0';
				return LINE_OK;
			}else {
				return LINE_BAD;
			}
		}
	}

	return LINE_OPEN;
	
}


// 分析请求行 逐个字符串分析
HTTP_CODE parse_requestline(char* temp, CHECK_STATE& checkstate) {
	/* C 库函数 char *strpbrk(const char *str1, const char *str2) 检索字符串 str1 中第一个匹配字符串 str2 中字符的字符，不包含空结束字符。也就是说，依次检验字符串 str1 中的字符，当被检验字符在字符串 str2 中也包含时，则停止检验，并返回该字符位置。*/
	// 匹配空格或者'\t'
	char* url = std::strpbrk(temp, " \t");
	if(url == nullptr) {
		return BAD_REQUEST;
	}
	*url++ = '\0';

	char* method = temp;
	if (strcasecmp(method, "GET") == 0)  {   /* 仅支持GET方法: 扩展？ 再加一个状态机 */
		printf("The request method is GET \n");
	}else {
		return BAD_REQUEST;
	}

	/*  检索字符串 str1 中第一个不在字符串 str2 中出现的字符下标。 */
	url += strspn(url, "\t");
	char* version = strpbrk(url, " \t");
	if(!version) {
		return BAD_REQUEST;
	}
	*version++ = '\0';

	version += strspn(version, " \t");
	/* 仅支持 HTTP1.1 */
	if(strcasecmp(version, "HTTP/1.1") != 0) {
		return BAD_REQUEST;
	}

	/* 检查URL是否合法 */
	if(strncasecmp(url, "http://", 7) == 0) {
		url += 7;
		url = strchr(url, '/');
	}

	if(url == nullptr || url[0] != '/') {
		return BAD_REQUEST;
	}

	printf("The request URL is : %s\n", url);
	/* HTTP请求行处理完毕， 状态转移到头部字段的分析 */
	checkstate = CHECK_STATE_HEADER;
	return NO_REQUEST;
}

HTTP_CODE parse_header(char *temp) {
	
}

HTTP_CODE parse_content() {
	// todo
}

int main() {
	
}