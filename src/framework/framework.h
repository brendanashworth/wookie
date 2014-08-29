// framework.h
#import "framework.c"
#import "../server.h"
#import "../http_parser/parser.h"

struct wookie_framework;
struct wookie_route;

struct wookie_request;
struct wookie_response;

/* Create new framework */
struct wookie_framework *wookie_new_framework(char *host, int port);

/* Add a route to the framework */
void wookie_add_route(struct wookie_framework*, struct wookie_route*);

/* Start the framework */
int wookie_start_framework(struct wookie_framework*);