// framework.h
#ifndef _FRAMEWORK_H
#define _FRAMEWORK_H

#include "http-parser/http_parser.c"

typedef struct wookie_framework wookie_framework;
typedef struct wookie_route wookie_route;

/**
 * Handle a request to the framework.
 * @param  arg http_request request.
 */
void wookie_framework_request(http_parser *parser);

/**
 * Creates a new wookie framework.
 * @param  host String representation of the host to listen on.
 * @param  port Port to listen on.
 * @return      A new wookie framework; must be w_free()'d.
 */
wookie_framework *wookie_new_framework(char *host, int port);

/**
 * Adds a new route to the framework.
 * @param framework The framework to add the route to.
 * @param route     The route to add.
 */
void wookie_add_route(wookie_framework *framework, wookie_route *route);

/**
 * Starts the framework server.
 * @param  framework The framework to start with.
 * @return           The framework's return value.
 */
int wookie_start_framework(wookie_framework *framework);

#include "framework.c"
#endif