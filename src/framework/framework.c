// framework.c

typedef struct {

} wookie_response;

typedef struct {
	char *path;
	http_request_type reqtype;
	void *(*call_route)(wookie_request*, wookie_response*);
} wookie_route;

struct wookie_framework {
	struct wookie_server *server;
	char *host;
	int port;
	wookie_route *routes;
	int routes_length;
};

wookie_framework *wookie_new_framework(char *host, int port) {
	wookie_framework *framework = malloc(sizeof(wookie_framework*));

	framework->host = host;
	framework->port = port;
	framework->routes = malloc(0);
	framework->routes_length = 0;

	return framework;
}

void wookie_add_route(wookie_framework *framework, wookie_route *route) {
	framework->routes = realloc(framework->routes, sizeof(framework->routes) + sizeof(wookie_route*));
	framework->routes[sizeof(framework->routes) / sizeof(wookie_route*)] = *route;
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
	for (int i = 0; i < framework->routes_length; i++) {
		wookie_route *route = malloc(sizeof(wookie_route*));
		route = &framework->routes[i];

		// check path
		if (strncmp(req->parsed_request->path, route->path, strlen(route->path) == 0)) {
			printf("IT matched :D\n");
			char *message = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 56\r\n\r\n<html><body><h1>WOOKIE HTTP SERVER :D</h1></body></html>\r\n";
			send(client->connfd, message, strlen(message), 0);
			break;
		} else {
			printf("Unmatched %s & %s\n", req->parsed_request->path, route->path);
		}
	}

	return NULL;
}
