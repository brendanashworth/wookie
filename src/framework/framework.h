// framework.h
#ifndef HEADER_FRAMEWORK
#define HEADER_FRAMEWORK

// forward declaration
typedef struct wookie_framework wookie_framework;

#include "../http_parser/parser.h"
#include "../server.h"
#include "framework.c"

/* Create new framework */
wookie_framework *wookie_new_framework(char *host, int port);

/* Add a route to the framework */
void wookie_add_route(wookie_framework*, wookie_route*);

/* Start the framework */
int wookie_start_framework(wookie_framework*);

#endif