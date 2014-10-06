// framework.c

#define FRAMEWORK_MAX_ROUTES 100

typedef struct {
	char *path;
	http_request_type reqtype;
	void *(*call_route)(wookie_request*);
} wookie_route;

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
	// go!
	printf("Starting wookie HTTP server on %s:%d\n", framework->host, framework->port);
	return wookie_start_server(framework, framework->host, framework->port);
}

void *wookie_framework_request(void *arg) {
	wookie_request *req = (wookie_request*)arg;
	wookie_framework *framework = req->client->server->framework;
	wookie_client *client = req->client;

	// iterate through routes, checking if path is EQUAL (not yet to regexes!)
	int handled = 0;
	for (int i = 0; i < framework->routes_length; i++) {
		// check path
		if (strncmp(req->parsed_request->path, framework->routes[i]->path, sizeof(&framework->routes[i]->path)) == 0) {
			// call
			framework->routes[i]->call_route(req);

			#ifdef MULTITHREADING
			w_free(req->client);
			w_free(req->parsed_request->path);
			w_free(req->parsed_request);
			w_free(req);
			#endif
			handled = 1;
			break;
		}
	}

	// must we send 404?
	if (!handled) {
		// send 404
		char *message = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: 79\r\n\r\n<html><body><h1>wookie HTTP framework</h1><h2>404: not found</h2></body></html>\r\n";
		send(client->connfd, message, strlen(message), 0);
		close(client->connfd);

		#ifdef MULTITHREADING
		w_free(req->client);
		w_free(req->parsed_request->path);
		w_free(req->parsed_request);
		w_free(req);
		#endif
	}

	#ifdef MULTITHREADING
	pthread_exit(0);
	#endif

	return NULL;
}
