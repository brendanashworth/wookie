// framework.c

typedef struct {

} wookie_request;

typedef struct {

} wookie_response;

typedef struct {
	char *path;
	http_request_type reqtype;
	void *(*call_route)(wookie_request*, wookie_response*);
} wookie_route;

typedef struct {
	struct wookie_server *server;
	char *host;
	int port;
	wookie_route *routes;
} wookie_framework;

wookie_framework *wookie_new_framework(char *host, int port) {
	wookie_framework *framework = malloc(sizeof(wookie_framework*));

	framework->host = malloc(strlen(host) + 1);
	strcpy(framework->host, host);
	framework->port = port;
	framework->routes = malloc(0);

	return framework;
}

void wookie_add_route(wookie_framework *framework, wookie_route *route) {
	framework->routes = realloc(framework->routes, sizeof(framework->routes) + sizeof(wookie_route*));
	framework->routes[sizeof(framework->routes) / sizeof(wookie_route*)] = *route;
}

int wookie_start_framework(wookie_framework *framework) {
	// go!
	printf("Starting wookie HTTP server on %s:%d\n", framework->host, framework->port);
	return wookie_start_server(framework->host, framework->port);
}