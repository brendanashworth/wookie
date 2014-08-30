// framework.c

struct wookie_framework {
	struct wookie_server *server;
	char *host;
	int port;
};

struct wookie_route {
	char *path;
	enum http_request_type reqtype;
	void *(*call_route)(struct wookie_request*, struct wookie_response*);
};

struct wookie_request {

};

struct wookie_response {

};

struct wookie_framework *wookie_new_framework(char *host, int port) {
	struct wookie_framework *framework = malloc(sizeof(struct wookie_framework*));

	framework->host = malloc(strlen(host) + 1);
	strcpy(framework->host, host);
	framework->port = port;

	return framework;
}