// application.c
#include <stdlib.h>
#include <string.h>

#include "../../src/framework.h"

void *handle_request(wookie_request *request) {
	// send answer
	wookie_response *response = w_malloc(sizeof *response);
	response->code = 200;
	response->content = "<html><body><h1>Example HTTP response, from wookie server.</h1></body></html>";

	http_response_send(response, request->client->connfd);

	w_free(response);	
	close(request->client->connfd);
	return NULL;
}

int main(int argc, char *argv[]) {
	// must provide host and port
	if (argc != 3) {
		printf("Usage: wookie [host] [port].\n");
		return 1;
	}

	wookie_framework *framework = w_malloc(sizeof *framework);

	// make framework, first arg: host, second arg: port
	framework = wookie_new_framework(argv[1], atoi(argv[2]));

	// add routes
	wookie_route *route = w_malloc(sizeof *route);
	route->path = "/";
	route->reqtype = HTTP_GET;
	route->call_route = &handle_request;
	wookie_add_route(framework, route);

	// start
	int result = wookie_start_framework(framework);

	// clear memory
	w_free(framework);
	w_free(route);

	return result;
}
