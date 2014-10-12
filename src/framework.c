// framework.c
#include <stdio.h>

// Must not only include stuff used by the framework.c file, but also application.c
#include "memory.h"
#include "server.h"
#include "http_response.h"

#define FRAMEWORK_MAX_ROUTES 100

// Representation of a route in wookie (to be replaced with r3)
struct wookie_route {
	char *path;
	enum http_method type;
	void *(*call_route)(wookie_request*, wookie_response*);
};

// Representation of the framework itself
struct wookie_framework {
	struct wookie_server *server;
	char *host;
	int port;
	wookie_route **routes;
	int routes_length;
};

wookie_framework *wookie_new_framework(char *host, int port) {
	wookie_framework *framework = w_malloc(sizeof *framework);

	framework->host = host;
	framework->port = port;
	framework->routes = w_malloc(FRAMEWORK_MAX_ROUTES * sizeof(wookie_route*)); // allocate for 100 routes
	framework->routes_length = 0;

	return framework;
}

void wookie_add_route(wookie_framework *framework, wookie_route *route) {
	// check if over route
	if (framework->routes_length >= FRAMEWORK_MAX_ROUTES) {
		printf("ERROR: Too many routes defined. Maximum routes allowed: %d\n", FRAMEWORK_MAX_ROUTES);
		return;
	}

	// reallocate for object and copy over
	framework->routes[framework->routes_length] = w_malloc(sizeof(wookie_route));
	framework->routes[framework->routes_length] = route;
	framework->routes_length++;
}

int wookie_start_framework(wookie_framework *framework) {
	// Go!
	printf("Starting wookie HTTP server on %s:%d...\n", framework->host, framework->port);
	return wookie_start_server(framework, framework->host, framework->port);
}

void wookie_framework_request(http_parser *parser) {
	// HOLD UP BITCH
	// WE GOT A NEW BETA FEATURE TO TEST

	printf("Received request\n");

	// Get both the request and response
	/*wookie_request *req = (wookie_request*)arg;
	wookie_response *response = w_malloc(sizeof *response);

	wookie_framework *framework = req->client->server->framework;

	// Iterate through routes, checking if path is EQUAL (not yet to regexes!)
	int handled = 0;
	for (int i = 0; i < framework->routes_length; i++) {
		// Check if the path matches
		if (strncmp(req->parsed_request->path, framework->routes[i]->path, sizeof(&framework->routes[i]->path)) == 0) {
			// Call the route, supply both the request and response.
			framework->routes[i]->call_route(req, response);

			handled = 1;
			break;
		}
	}

	if (!handled) {
		// Send 404
		response->code = "404";
		response->content = "<html><body><h1>wookie HTTP framework</h1><h2>404: not found</h2></body></html>";
	}

	// Send HTTP response and close connection
	http_response_send(response, req->client->connfd);
	close(req->client->connfd);

	// The request is done - cleanup
	w_free(req->client);
	w_free(req->parsed_request->path);
	w_free(req->parsed_request);
	w_free(req);

	free(response);

	pthread_exit(0);
	return NULL;*/
}
