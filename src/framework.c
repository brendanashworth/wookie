// framework.c

#define FRAMEWORK_MAX_ROUTES 100

struct wookie_route {
	char *path;
	http_request_type reqtype;
	void *(*call_route)(wookie_request*, wookie_response*);
};

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

	// reallocate for new object
	framework->routes[framework->routes_length] = w_malloc(sizeof(wookie_route));
	// copy over route
	framework->routes[framework->routes_length]->path = route->path;
	framework->routes[framework->routes_length]->reqtype = route->reqtype;
	framework->routes[framework->routes_length]->call_route = route->call_route;
	framework->routes_length++;
}

int wookie_start_framework(wookie_framework *framework) {
	// Go!
	printf("Starting wookie HTTP server on %s:%d\n", framework->host, framework->port);
	return wookie_start_server(framework, framework->host, framework->port);
}

void *wookie_framework_request(void *arg) {
	// Get both the request and response
	wookie_request *req = (wookie_request*)arg;
	wookie_response *response = w_malloc(sizeof *response);

	wookie_framework *framework = req->client->server->framework;
	//wookie_client *client = req->client;

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

	#ifdef MULTITHREADING
	w_free(req->client);
	w_free(req->parsed_request->path);
	w_free(req->parsed_request);
	w_free(req);

	pthread_exit(0);
	#endif

	return NULL;
}
