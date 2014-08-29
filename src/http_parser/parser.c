// parser.c
#import <stdio.h>
#import <string.h>
#import <stdlib.h>

enum http_request_type {
	HTTP_GET,
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

void parser_parse_requestline(struct parsed_result *result, char *orig_request) {
	char *request = orig_request;

	// parse method
	// GET
	if (strncmp(request, "GET ", 4) == 0)
		result->request_type = HTTP_GET;
	else {
		printf("Bad request #1.\n");
		return;
	}

	// parse path
	request += 4;

	result->path = malloc(1);
	result->path[0] = '\0'; // null terminate

	// continue until there is a space
	for (int i = 0; 1; i++) {
		if (request[0] == ' ')
			break;
		else {
			result->path = realloc(result->path, sizeof(result->path) + 1);
			result->path[i] = request[i];
		}

		request++;
	}

	// parse version
	request++; // shift off space

	if (strncmp(request, "HTTP/1.1", 8) == 0)
		result->version = VERSION_ONE_ONE;
	else if (strncmp(request, "HTTP/1.0", 8) == 0)
		result->version = VERSION_ONE_ZERO;
	else {
		printf("Bad request #2.\n");
		printf("%s\n", request);
		return;
	}

	return;
}

struct parsed_result *parser_parse(char *request) {
	struct parsed_result *result = malloc(sizeof(struct parsed_result*));

	// parse request line
	parser_parse_requestline(result, request);

	//parser_parse_headers()

	return result;
}
