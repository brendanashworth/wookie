// application.c
#include <stdlib.h>
#include <string.h>

#include "../../src/framework/framework.h"

void *handle_request(wookie_request *request) {
	// send answer
	char *message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 94\r\n\r\n<html><body><h1>Example HTTP response, from the Wookie example application.</h1></body></html>\r\n";
	send(request->client->connfd, message, strlen(message), 0);
	
	close(request->client->connfd);

	return NULL;
}

int main(int argc, char *argv[]) {
	// must provide host and port
	if (argc != 3) {
		printf("Usage: wookie [host] [port].\n");
		return 1;
	}

	wookie_framework *framework = malloc(sizeof *framework);

	// make framework, first arg: host, second arg: port
	framework = wookie_new_framework(argv[1], atoi(argv[2]));

	// add routes
	wookie_route *route = malloc(sizeof *route);
	route->path = "/";
	route->reqtype = HTTP_GET;
	route->call_route = &handle_request;
	wookie_add_route(framework, route);

	// start
	int result = wookie_start_framework(framework);

	// clear memory
	free(framework);
	free(route);

	return result;
}
