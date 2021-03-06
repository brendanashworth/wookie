// server.h
#ifndef _SERVER_H
#define _SERVER_H

typedef struct wookie_request wookie_request;
typedef struct wookie_client wookie_client;
typedef struct wookie_server wookie_server;

/**
 * Starts a wookie server.
 * @param  framework Framework object.
 * @param  host      Host to listen on.
 * @param  port      Port to listen on.
 * @return           Program response code.
 */
int wookie_start_server(wookie_framework *framework, char *host, int port);

/**
 * Handles a client in wookie.
 * @param  client The client.
 */
void wookie_handle_client(wookie_client *client);

#include "server.c"
#endif