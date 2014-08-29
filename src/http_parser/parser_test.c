// parser_test.c
#include "parser.h"

int main() {
	char *request = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
	struct parsed_result *result = parser_parse(request);

	free(result);

	return 0;
}