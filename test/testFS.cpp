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

// ��״̬�������ڽ���һ�����ݣ����ؽ�������״̬��
LINE_STATUE parse_line(char* buffer, int& checked_index, int& read_index){
	char temp;
	/* check_index ָ��buffer���ڶ����ֽڣ� read_indexָ��ͻ�����β������һ���ֽڣ� 0-check_index���Ѿ��������ꡣ �� check_index - read_index �ֽ�������ѭ������*/

	for( ; checked_index < read_index; ++checked_index) {
		temp = buffer[checked_index];
		// �س��У���ô֮ǰ�����Ŀ�����������
		if(temp == '\r') {
			// ������ͽ���������������
			if((checked_index + 1) == read_index) {
				return LINE_OPEN;
			}else if(buffer[checked_index + 1] == '\n') {
				buffer[checked_index++] = '\0';
				buffer[checked_index++] = '\0';
				return LINE_OK;
			}else {
				// �﷨����
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


// ���������� ����ַ�������
HTTP_CODE parse_requestline(char* temp, CHECK_STATE& checkstate) {
	/* C �⺯�� char *strpbrk(const char *str1, const char *str2) �����ַ��� str1 �е�һ��ƥ���ַ��� str2 ���ַ����ַ����������ս����ַ���Ҳ����˵�����μ����ַ��� str1 �е��ַ������������ַ����ַ��� str2 ��Ҳ����ʱ����ֹͣ���飬�����ظ��ַ�λ�á�*/
	// ƥ��ո����'\t'
	char* url = std::strpbrk(temp, " \t");
	if(url == nullptr) {
		return BAD_REQUEST;
	}
	*url++ = '\0';

	char* method = temp;
	if (strcasecmp(method, "GET") == 0)  {   /* ��֧��GET����: ��չ�� �ټ�һ��״̬�� */
		printf("The request method is GET \n");
	}else {
		return BAD_REQUEST;
	}

	/*  �����ַ��� str1 �е�һ�������ַ��� str2 �г��ֵ��ַ��±ꡣ */
	url += strspn(url, "\t");
	char* version = strpbrk(url, " \t");
	if(!version) {
		return BAD_REQUEST;
	}
	*version++ = '\0';

	version += strspn(version, " \t");
	/* ��֧�� HTTP1.1 */
	if(strcasecmp(version, "HTTP/1.1") != 0) {
		return BAD_REQUEST;
	}

	/* ���URL�Ƿ�Ϸ� */
	if(strncasecmp(url, "http://", 7) == 0) {
		url += 7;
		url = strchr(url, '/');
	}

	if(url == nullptr || url[0] != '/') {
		return BAD_REQUEST;
	}

	printf("The request URL is : %s\n", url);
	/* HTTP�����д�����ϣ� ״̬ת�Ƶ�ͷ���ֶεķ��� */
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