// parser.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// for extreme performance, you can tailor-fit the parser.
// comment out if you are not using a section
#define HTTP_CHECK_PATH

enum http_request_type {
	HTTP_GET,
	HTTP_POST,
};

enum http_version {
	VERSION_ONE_ONE,
	VERSION_ONE_ZERO,
};

struct parsed_result {
	http_request_type request_type;
	char *path;
	http_version version;
};

int parser_parse_requestline(parsed_result *result, char *orig_request) {
	char *request = orig_request;

	// parse method
	// GET
	if (strncmp(request, "GET ", 4) == 0) {
		result->request_type = HTTP_GET;
		request += 4;
	} else if (strncmp(request, "POST ", 5) == 0) {
		result->request_type = HTTP_POST;
		request += 5;
	} else {
		return -1;
	}

	// parse path
	result->path = w_malloc(1); // we don't null terminate until after the for loop

	// continue until there is a space
	for (int i = 0; request[0] != ' '; i++) {
		result->path = w_realloc(result->path, sizeof(result->path) + 2); // allocate one for new char and one for \0
		result->path[i] = request[i];

		request++; // shift pointer
	}
	result->path[sizeof(result->path) + 1] = '\0'; // null terminate
	request++; // shift off space

	#ifdef HTTP_CHECK_PATH
	/* checks to make sure it starts with a '/', doesn't contain null chars, etc */
	if (result->path[0] != '/')
		return -2;

	// check for characters: 65-122, numbers?, 
	//else if (result->path[strlen(result->path)])

	#endif

	// parse version

	if (strncmp(request, "HTTP/1.1", 8) == 0)
		result->version = VERSION_ONE_ONE;
	else if (strncmp(request, "HTTP/1.0", 8) == 0)
		result->version = VERSION_ONE_ZERO;
	else
		return -4;

	return 0;
}

parsed_result *parser_parse(char *request) {
	parsed_result *result = w_malloc(sizeof *result);

	// parse request line
	int err = parser_parse_requestline(result, request);
	if (err != 0)
		printf("Error parsing request line: bad request %d\n", err);

	//parser_parse_headers()

	return result;
}
