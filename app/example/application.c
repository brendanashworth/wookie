// application.c
#include <stdlib.h>
#include <string.h>

#import "../../src/framework/framework.h"

void *handle_request(struct wookie_request *request, struct wookie_response *response) {
	// do something
}

int main(int argc, char *argv[]) {
	// must provide host and port
	if (argc != 3) {
		printf("Usage: wookie [host] [port].\n");
		return 1;
	}

	// host and port
	char *host = argv[1];
	int port = atoi(argv[2]);

	struct wookie_framework *framework = malloc(sizeof(struct wookie_framework*));

	// make framework
	framework = wookie_new_framework(host, port);

	// add routes
	struct wookie_route *route = malloc(sizeof(struct wookie_route*));
	route->path = "/";
	route->reqtype = HTTP_GET;
	route->call_route = &handle_request;
	wookie_add_route(framework, route);

	// start
	int result = wookie_start_framework(framework);
	return result;
}
