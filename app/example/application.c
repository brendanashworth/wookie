// application.c
#include <stdlib.h>
#include <string.h>

#include "framework.h"

void *handle_request(wookie_request *request, wookie_response *response) {
	// Send answer
	response->code = "200";
	response->content = "<html><body><h1>Example HTTP response, from wookie server.</h1></body></html>";

	return NULL;
}

int main(int argc, char *argv[]) {
	// must provide host and port
	if (argc != 3) {
		printf("Usage: wookie [host] [port].\n");
		return 1;
	}

	// make framework, first arg: host, second arg: port
	wookie_framework *framework = wookie_new_framework(argv[1], atoi(argv[2]));

	// add routes
	wookie_route *route = w_malloc(sizeof *route);
	route->path = "/";
	route->type = HTTP_GET;
	route->call_route = &handle_request;
	wookie_add_route(framework, route);

	// start
	int result = wookie_start_framework(framework);

	// clear memory
	w_free(framework);
	w_free(route);

	return result;
}
