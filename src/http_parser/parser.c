// parser.c
#import <string.h>
#import <stdlib.h>

// for extreme performance, you can tailor-fit which methods you are using.
// comment out if you are not using a method
#define REQTYPE_HTTP_POST

enum http_request_type {
	HTTP_GET,
	#ifdef REQTYPE_HTTP_POST
	HTTP_POST,
	#endif
};

enum http_version {
	VERSION_ONE_ONE,
	VERSION_ONE_ZERO,
};

struct parsed_result {
	enum http_request_type request_type;
	char *path;
	enum http_version version;
};

int parser_parse_requestline(struct parsed_result *result, char *orig_request) {
	char *request = orig_request;

	// parse method
	// GET
	if (strncmp(request, "GET ", 4) == 0) {
		result->request_type = HTTP_GET;
		request += 4;
	#ifdef REQTYPE_HTTP_POST
	} else if (strncmp(request, "POST ", 5) == 0) {
		result->request_type = HTTP_POST;
		request += 5;
	#endif
	} else {
		return -1;
	}

	// parse path
	result->path = malloc(1);
	//result->path[0] = '\0'; // null terminate

	// continue until there is a space
	for (int i = 0; request[0] != ' '; i++) {
		result->path = realloc(result->path, sizeof(result->path) + 2); // allocate one for new char and one for \0
		result->path[i] = request[i];

		request++; // shift pointer
	}
	result->path[sizeof(result->path) + 1] = '\0'; // null terminate
	request++; // shift off space

	// parse version

	if (strncmp(request, "HTTP/1.1", 8) == 0)
		result->version = VERSION_ONE_ONE;
	else if (strncmp(request, "HTTP/1.0", 8) == 0)
		result->version = VERSION_ONE_ZERO;
	else
		return -2;

	return 0;
}

struct parsed_result *parser_parse(char *request) {
	struct parsed_result *result = malloc(sizeof(struct parsed_result*));

	// parse request line
	int err = parser_parse_requestline(result, request);
	if (err < 0)
		printf("Error parsing request line: bad request %d\n", err);

	//parser_parse_headers()

	return result;
}
