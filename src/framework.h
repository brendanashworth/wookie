// framework.h
#ifndef _FRAMEWORK_H
#define _FRAMEWORK_H

typedef struct wookie_framework wookie_framework;
typedef struct wookie_route wookie_route;

/* Send an HTTP request back to the framework */
void *wookie_framework_request(void*);

// Include everything.
// First utilities
#include "memory.h"

// Then the HTTP stuff
#include "parser.h"
#include "http_response.h"

// Then the server & framework
#include "server.h"

/* Create new framework */
wookie_framework *wookie_new_framework(char*, int);

/* Add a route to the framework */
void wookie_add_route(wookie_framework*, wookie_route*);

/* Start the framework */
int wookie_start_framework(wookie_framework*);

#include "framework.c"
#endif